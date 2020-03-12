#ifndef RECEIVERCONTENT_H
#define RECEIVERCONTENT_H

#include "receiverworker.h"
#include "ui/button.h"
#include "ui/recordbutton.h"
#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QTreeWidget>

#include <maya/MCallbackIdArray.h>

class ReceiverContent : public QWidget
{
    Q_OBJECT
public:
    ReceiverContent(QWidget* parent=nullptr);
    ~ReceiverContent();

    void reset();
private:
    void onReceiverToggled(bool);
    QSpinBox* portBox=nullptr;
    DataReceivingWorker* worker=nullptr;
    QLabel* statusLabel=nullptr;
    Button* startReceiverBtn=nullptr;
    QTreeWidget* treeWidget=nullptr;
    RecordButton* startRecordingBtn=nullptr;
    void prepareContextMenu(const QPoint &pos);
    void recordingToggled(bool checked);
    void populateTree();
    void clearTreeWidget();

    MCallbackIdArray callbacks;
};

#endif // RECEIVERCONTENT_H
