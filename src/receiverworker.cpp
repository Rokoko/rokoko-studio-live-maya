#include "receiverworker.h"
#include "animations.h"
#include "animations.h"
#include <QFile>
#include <QJsonArray>
#include "lz4frame.h"
#include "jsonv2parser.h"
#include "jsonv3parser.h"


DataReceivingWorker::DataReceivingWorker(QObject* parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);

    connect(socket, &QUdpSocket::connected, this, &DataReceivingWorker::onSocketConnected);
    connect(socket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::errorOccurred), this, &DataReceivingWorker::onSocketError);

    connect(&hearbeat, &QTimer::timeout, this, &DataReceivingWorker::onHeartBeat);
    hearbeat.setInterval(1000 / RECEIVER_FPS);
    hearbeat.start();
}

DataReceivingWorker::~DataReceivingWorker()
{
    if(parserImpl != nullptr)
    {
        delete parserImpl;
        parserImpl = nullptr;
    }
}

void DataReceivingWorker::pause()
{
    socket->abort();
}

const std::vector<FPropSnapshot> DataReceivingWorker::getPropSnapshots()
{
    if(parserImpl != nullptr)
        return parserImpl->propSnapshots();
    return {};
}

const std::vector<FActorSnapshot> DataReceivingWorker::actorSnapshots()
{
    if(parserImpl != nullptr)
        return parserImpl->actorSnapshots();
    return {};
}

void DataReceivingWorker::onSocketError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);

    emit workerStateChanged(QString("Socket error! %1").arg(socket->errorString()));

}

void DataReceivingWorker::start(int port) {
    if(!socket->bind(port, QAbstractSocket::ReuseAddressHint)){
        emit workerStateChanged("Not connected!");
    } else {
        emit onSocketConnected();
    }
}

void DataReceivingWorker::readAndApplyData()
{
    if(socket->state() != QAbstractSocket::SocketState::BoundState) {
        emit workerStateChanged("Socket not connected!");
        return;
    }

    // 1. read datagram and prepare source bytes array
    const qint64 pendingSize = socket->pendingDatagramSize();
    if(pendingSize <= 0)
    {
        emit workerStateChanged("Receiving no data!");
        return;
    }
    QByteArray datagram;
    datagram.resize(pendingSize);
    const qint64 datagramReadLen = socket->readDatagram(datagram.data(), datagram.size());



    // 2. detect protocol version
    float protocolVersionNumber = -1.0;
    if(const char* versionKw = strstr(datagram.data(), "version"))
    {
        const char* version2Num = strstr(versionKw, "2,");
        const char* version25Num = strstr(versionKw, "2.5,");
        if(version2Num || version25Num)
        {
            protocolVersionNumber = version25Num == nullptr ? 2.0 : 2.5;
        }
    } else {
        protocolVersionNumber = 3.0;
    }

    const bool versionIsValid = protocolVersionNumber > 0;



    // 3. Pick implementation
    if (versionIsValid)
    {
        if(protocolVersionNumber != mLastProtocolVersion)
        {
            // change implemention when version changes
            if(parserImpl != nullptr)
            {
                delete parserImpl;
                parserImpl = nullptr;
            }

            if(protocolVersionNumber == 2.0)
            {
                printf("Pick JsonV2 parser! Versions: %f - %f\n", mLastProtocolVersion, protocolVersionNumber);
                parserImpl = new JsonV2Parser();
            } else if(protocolVersionNumber == 3.0)
            {
                printf("Pick JsonV3 parser!\n");
                parserImpl = new JsonV3Parser();
            }
        }

        mLastProtocolVersion = protocolVersionNumber;
    }

    // 4. Feed received bytes to implementation
    if(parserImpl && versionIsValid)
    {
        emit workerStateChanged("Working");
        parserImpl->feed(datagram, datagramReadLen);
        parserImpl->parse();
        parserImpl->apply();
    }
}

void DataReceivingWorker::onHeartBeat()
{
    readAndApplyData();
}
