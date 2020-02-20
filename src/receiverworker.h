#ifndef RECEIVERWORKER_H
#define RECEIVERWORKER_H

#include "constants.h"
#include "singleton.h"
#include <QObject>
#include <QUdpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>


class DataReceivingWorker : public QObject
{
    Q_OBJECT
public:
    DataReceivingWorker(QObject* parent=nullptr);
public slots:
    void start(int port=DEFAULT_RS_PORT);
    void pause();
signals:
    void workerStateChanged(QString);
    void onSocketConnected();

private:
    void processData(QJsonObject);
    void onSocketError(QAbstractSocket::SocketError);
    QUdpSocket* socket = nullptr;
    void readData();
    QTimer hearbeat;
    void onHearBeat();
};

#endif // RECEIVERWORKER_H
