#ifndef RECEIVERWORKER_H
#define RECEIVERWORKER_H

#include "constants.h"
#include "singleton.h"
#include <QObject>
#include <QUdpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include "lz4frame.h"


class DataReceivingWorker : public QObject
{
    Q_OBJECT
public:
    DataReceivingWorker(QObject* parent=nullptr);
    ~DataReceivingWorker();
public slots:
    void start(int port=DEFAULT_RS_PORT);
    void pause();
signals:
    void workerStateChanged(QString);
    void onSocketConnected();

private:
    void parseData(QJsonObject);
    void onSocketError(QAbstractSocket::SocketError);
    QUdpSocket* socket = nullptr;
    void readAndApplyData();
    QTimer hearbeat;
    void onHearBeat();

    LZ4F_decompressionContext_t mDecompressionContext = nullptr;
};

#endif // RECEIVERWORKER_H
