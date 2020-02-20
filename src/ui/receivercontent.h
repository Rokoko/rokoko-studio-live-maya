#ifndef RECEIVERCONTENT_H
#define RECEIVERCONTENT_H

#include "receiverworker.h"
#include <QWidget>
#include <QSpinBox>
#include <QLabel>

class ReceiverContent : public QWidget
{
    Q_OBJECT
public:
    ReceiverContent(QWidget* parent=nullptr);
    ~ReceiverContent();
public slots:
    void handleListenerError(QString);
private:
    void onReceiveToggled(bool);
    QSpinBox* portBox=nullptr;
    DataReceivingWorker* worker=nullptr;
    QLabel* testLabel=nullptr;
    QThread* listenerThread=nullptr;
};

#endif // RECEIVERCONTENT_H
