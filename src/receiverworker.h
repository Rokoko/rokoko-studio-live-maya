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
#include "interfaces.h"
#include "framework.h"


class DataReceivingWorker : public QObject
{
    Q_OBJECT
public:
    DataReceivingWorker(QObject* parent=nullptr);
    ~DataReceivingWorker();
public slots:
    void start(int port=DEFAULT_RS_PORT);
    void pause();
    const std::vector<FPropSnapshot> getPropSnapshots();
    const std::vector<FActorSnapshot> actorSnapshots();
signals:
    void workerStateChanged(QString);
    void onSocketConnected();

private:
    void onSocketError(QAbstractSocket::SocketError);
    QUdpSocket* socket = nullptr;
    void readAndApplyData();
    QTimer hearbeat;
    void onHeartBeat();

    LZ4F_decompressionContext_t mDecompressionContext = nullptr;
    char* mDecompressionBuffer = nullptr;

    IDataFormatParser* parserImpl = nullptr;
    float mLastProtocolVersion = -1.0;
};

#endif // RECEIVERWORKER_H
