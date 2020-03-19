#include "receiverworker.h"
#include "animations.h"
#include "animations.h"
#include <QFile>
#include <QJsonArray>

DataReceivingWorker::DataReceivingWorker(QObject* parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);

    connect(socket, &QUdpSocket::connected, this, &DataReceivingWorker::onSocketConnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &DataReceivingWorker::onSocketError);

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
        facesMap[face["faceId"].toString()] = faceObject;
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

    if(socket->pendingDatagramSize() == -1) {
        emit workerStateChanged("Receiving no data!");
        return;
    }

    QByteArray datagram;
    datagram.resize(socket->pendingDatagramSize());
    socket->readDatagram(datagram.data(), datagram.size());

    QJsonDocument doc = QJsonDocument::fromJson(datagram.data());

    QJsonObject frameData = doc.object();

    int protocolVersion = frameData["version"].toInt();
    if(protocolVersion != 2) {
        emit workerStateChanged("Not valid data format! Use JSON v2!");
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
