#ifndef RECEIVERCONTENT_H
#define RECEIVERCONTENT_H

#include "receiverworker.h"
#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QTreeWidget>

class ReceiverContent : public QWidget
{
    Q_OBJECT
public:
    ReceiverContent(QWidget* parent=nullptr);
    ~ReceiverContent();
private:
    void onReceiveToggled(bool);
    QSpinBox* portBox=nullptr;
    DataReceivingWorker* worker=nullptr;
    QLabel* statusLabel=nullptr;
    QTreeWidget* treeWidget=nullptr;
    void populateTree();
    void clearTreeWidget();
};

#endif // RECEIVERCONTENT_H
