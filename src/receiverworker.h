#ifndef RECEIVERWORKER_H
#define RECEIVERWORKER_H

#include "constants.h"
#include "singleton.h"
#include <QObject>
#include <QUdpSocket>
#include <QJsonDocument>
#include <QJsonObject>


class DataReceivingWorker : public QObject
{
    Q_OBJECT
public:
    DataReceivingWorker(QObject* parent=nullptr);
public slots:
    void start(int port=DEFAULT_RS_PORT);
    void pause();
signals:
    void onError(QString);
    void frameReceived(QJsonObject);
private:
    void onSocketError(QAbstractSocket::SocketError);
    QUdpSocket* socket = nullptr;
    void readData();
};

#endif // RECEIVERWORKER_H
