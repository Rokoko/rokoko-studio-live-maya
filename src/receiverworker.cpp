#include <iostream>
#include "receiverworker.h"
#include "animations.h"
#include <iostream>
#include <QFile>
#include <QJsonArray>

DataReceivingWorker::DataReceivingWorker(QObject* parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);

    connect(socket, &QUdpSocket::connected, this, &DataReceivingWorker::onSocketConnected);
//    connect(socket, &QUdpSocket::readyRead, this, &DataReceivingWorker::readData);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &DataReceivingWorker::onSocketError);

    connect(&hearbeat, &QTimer::timeout, this, &DataReceivingWorker::onHearBeat);
    hearbeat.setInterval(33);
    hearbeat.start();
}

void DataReceivingWorker::pause() {
    socket->abort();
}

void DataReceivingWorker::processData(QJsonObject data)
{
    // parse props
    QJsonArray propsArray = data["props"].toArray();
    QHash<QString, QJsonObject> propsMap;
    foreach(const QJsonValue& value, propsArray) {
        QJsonObject prop = value.toObject();
        propsMap[prop["id"].toString()] = prop;
    }
    Animations::getInstance()->putProps(propsMap);

    // parse trackers
    QHash<QString, QJsonObject> trackersMap;
    QJsonArray trackersArray = data["trackers"].toArray();
    foreach(const QJsonValue& value, trackersArray) {
        QJsonObject tracker = value.toObject();
        trackersMap[tracker["name"].toString()] = tracker;
    }
    Animations::getInstance()->putTrackers(trackersMap);
}

void DataReceivingWorker::onSocketError(QAbstractSocket::SocketError err)
{
    emit workerStateChanged(QString("Socket error! %1").arg(socket->errorString()));

}

void DataReceivingWorker::start(int port) {
    if(!socket->bind(QHostAddress::LocalHost, port, QAbstractSocket::ReuseAddressHint)){
        emit workerStateChanged("Not sonnceted!");
    } else {
        emit onSocketConnected();
    }
}

void DataReceivingWorker::readData() {

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

    emit workerStateChanged("Working");
    processData(frameData);
}

void DataReceivingWorker::onHearBeat()
{
    readData();
}



