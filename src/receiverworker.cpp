#include "receiverworker.h"
#include <iostream>

DataReceivingWorker::DataReceivingWorker(QObject* parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);
    connect(socket, &QUdpSocket::readyRead, this, &DataReceivingWorker::readData);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &DataReceivingWorker::onSocketError);
}

void DataReceivingWorker::pause() {
    socket->close();
}

void DataReceivingWorker::onSocketError(QAbstractSocket::SocketError err)
{
    emit onError(socket->errorString());
}

void DataReceivingWorker::start(int port) {
    if(!socket->bind(QHostAddress::LocalHost, port, QAbstractSocket::ReuseAddressHint)){
        emit onError(socket->errorString());
    }
    if (socket->state() != QAbstractSocket::ConnectedState || QAbstractSocket::ConnectingState) {
        emit onError("Failed to connect!");
    }
}

void DataReceivingWorker::readData() {

    QByteArray datagram;
    datagram.resize(socket->pendingDatagramSize());
    socket->readDatagram(datagram.data(), datagram.size());

    QJsonDocument doc = QJsonDocument::fromJson(datagram.data());

    QJsonObject frameData = doc.object();

    int protocolVersion = frameData["version"].toInt();
    if(protocolVersion != 2) {
        emit onError("Wrong live data format! Use JSON v2!");
        pause();
        return;
    }

    emit frameReceived(frameData);

}
