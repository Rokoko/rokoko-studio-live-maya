#include "receiverworker.h"
#include "animations.h"
#include "animations.h"
#include <QFile>
#include <QJsonArray>
#include "lz4frame.h"


DataReceivingWorker::DataReceivingWorker(QObject* parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);

    connect(socket, &QUdpSocket::connected, this, &DataReceivingWorker::onSocketConnected);
    connect(socket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &DataReceivingWorker::onSocketError);

    connect(&hearbeat, &QTimer::timeout, this, &DataReceivingWorker::onHearBeat);
    hearbeat.setInterval(1000 / RECEIVER_FPS);
    hearbeat.start();
}

DataReceivingWorker::~DataReceivingWorker()
{

}

void DataReceivingWorker::pause() {
    socket->abort();
}

void DataReceivingWorker::parseData(QJsonObject data)
{
    // put timestamps
    Animations::get()->timestamp = data["timestamp"].toDouble();
    Animations::get()->playbackTimestamp = data["playbackTimestamp"].toDouble();

    // parse props
    QJsonArray propsArray = data["props"].toArray();
    QHash<QString, QJsonObject> propsMap;
    foreach(const QJsonValue& value, propsArray) {
        QJsonObject prop = value.toObject();
        propsMap[prop["id"].toString()] = prop;
    }
    Animations::get()->putProps(propsMap);

    // parse trackers
    QHash<QString, QJsonObject> trackersMap;
    QJsonArray trackersArray = data["trackers"].toArray();
    foreach(const QJsonValue& value, trackersArray) {
        QJsonObject tracker = value.toObject();
        trackersMap[tracker["name"].toString()] = tracker;
    }
    Animations::get()->putTrackers(trackersMap);

    // put faces
    QHash<QString, QJsonObject> facesMap;
    QJsonArray faceArray = data["faces"].toArray();
    foreach(auto face, faceArray) {
        QJsonObject faceObject = face.toObject();
        facesMap[faceObject["faceId"].toString()] = faceObject;
    }
    Animations::get()->putFaces(facesMap);

    // parse actors
    QHash<QString, QJsonObject> actorsMap;
    QJsonArray actorsArray = data["actors"].toArray();
    foreach(const QJsonValue& value, actorsArray) {
        QJsonObject actor = value.toObject();
        actorsMap[actor["id"].toString()] = actor;
    }
    Animations::get()->putActors(actorsMap);

}

void DataReceivingWorker::onSocketError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);

    emit workerStateChanged(QString("Socket error! %1").arg(socket->errorString()));

}

void DataReceivingWorker::start(int port) {
    if(!socket->bind(QHostAddress::LocalHost, port, QAbstractSocket::ReuseAddressHint)){
        emit workerStateChanged("Not connected!");
    } else {
        // TODO: fetch maya objects with mapping attribute
        emit onSocketConnected();
    }
}

void DataReceivingWorker::readAndApplyData() {

    if(socket->state() != QAbstractSocket::SocketState::BoundState) {
        emit workerStateChanged("Socket not connected!");
        return;
    }

    const qint64 pendingSize = socket->pendingDatagramSize();

    if(pendingSize <= 0)
    {
        emit workerStateChanged("Receiving no data!");
        pause();
        return;
    }

    QByteArray datagram;

    datagram.resize(pendingSize);
    const qint64 datagramReadLen = socket->readDatagram(datagram.data(), datagram.size());

    // detect protocol version
    float protocolVersionNumber = -1.0;
    if(const char* versionKw = strstr(datagram.data(), "version"))
    {
        const char* version2Num = strstr(versionKw, "2");
        const char* version25Num = strstr(versionKw, "2.5");
        const char* delim = strstr(versionKw, ",");
        if(version2Num && delim && version2Num < delim)
        {
            protocolVersionNumber = version25Num == nullptr ? 2.0 : 2.5;
        }
    } else {
        protocolVersionNumber = 3.0;
    }

    QJsonObject frameData;
    if(protocolVersionNumber == 2.0)
    {
        QJsonDocument doc = QJsonDocument::fromJson(datagram.data());
        frameData = doc.object();
    } else if(protocolVersionNumber == 3.0)
    {
        // decompress json3 and build frame data json object
        constexpr int bufferSize{ 8192 * 32 };
        if (!mDecompressionBuffer)
            mDecompressionBuffer = new char[bufferSize];

        if (!mDecompressionContext)
            LZ4F_createDecompressionContext(&mDecompressionContext, LZ4F_VERSION);

        LZ4F_resetDecompressionContext(mDecompressionContext);
        size_t srcLen = static_cast<size_t>(datagramReadLen);
        size_t dstLen = static_cast<size_t>(bufferSize);
        size_t status = LZ4F_decompress(mDecompressionContext, mDecompressionBuffer, &dstLen, datagram.data(), &srcLen, nullptr);
        if (status > 0)
        {
            emit workerStateChanged("JSON v3 data malformed!");
            return;
        } else {
            if (const char* keyword = strstr(mDecompressionBuffer, "version"))
            {
                const char* version3Num = strstr(keyword, "3,");
                if(version3Num != nullptr)
                {
                    // cut garbage from end
                    for (size_t i = dstLen; i > 0; --i)
                    {
                        if (mDecompressionBuffer[i] == '}')
                            break;
                        mDecompressionBuffer[i] = '\0';
                    }
                    // init frameData from decompressed string
                    printf("%s\n\n", mDecompressionBuffer);
                }
            }
        }

    }

    if(protocolVersionNumber < 0)
    {
        emit workerStateChanged("Not valid data format! Use JSON v2 or v3!");
        pause();
        return;
    }

    // TODO: emit only on change
    emit workerStateChanged("Working");

    // 1 pass: parse json into Animatins storage
    parseData(frameData);

    // 2 pass: mapping is dynamyc and user can map/unmap objects at orbitrary moment

    // 3 pass: apply data to currently mapped objects
    Animations::get()->applyAnimationsToMappedObjects();

}

void DataReceivingWorker::onHearBeat()
{
    readAndApplyData();
}
