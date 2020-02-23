#include "receivercontent.h"
#include "receiverworker.h"
#include "animations.h"
#include "ui/button.h"
#include "ui/recordbutton.h"

#include <QThread>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QJsonArray>
#include <QTimer>


ReceiverContent::ReceiverContent(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(1);

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->setContentsMargins(1, 1, 1, 1);
    formLayout->setSpacing(1);

    // port field
    portBox = new QSpinBox(this);
    portBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    portBox->setAlignment(Qt::AlignCenter);
    portBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    portBox->setMaximum(65535);
    portBox->setValue(14043);

    formLayout->addRow("Port:", portBox);

    // scene scale field
    QDoubleSpinBox* sceneScaleBox = new QDoubleSpinBox(this);
    sceneScaleBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sceneScaleBox->setAlignment(Qt::AlignCenter);
    sceneScaleBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    sceneScaleBox->setValue(1.0);
    formLayout->addRow("Scene scale:", sceneScaleBox);
    mainLayout->addLayout(formLayout);

    // reset scene on stop option
    QCheckBox* cbResetSceneOnStop = new QCheckBox("Reset Scene on Stop", this);
    mainLayout->addWidget(cbResetSceneOnStop);

    // hide meshes during play
    QCheckBox* cbHideMeshesDuringPlay = new QCheckBox("Hide Meshes during Play", this);
    mainLayout->addWidget(cbHideMeshesDuringPlay);

    // start receiver button
    ButtonParams receiverBtnParams;
    receiverBtnParams.iconSize = 16;
    receiverBtnParams.checkedIconPath = ":/resources/pause.svg";
    receiverBtnParams.uncheckedIconPath = ":/resources/play.svg";
    receiverBtnParams.checkedStateText = "Stop Receiver";
    receiverBtnParams.uncheckedStateText = "Start Receiver";
    receiverBtnParams.iconAlignment = Qt::AlignLeft;

    Button* startReceiverBtn = new Button(this, receiverBtnParams);
    startReceiverBtn->setCheckable(true);
    mainLayout->addWidget(startReceiverBtn);
    connect(startReceiverBtn, &Button::toggled, this, &ReceiverContent::onReceiveToggled);

    // record button
    RecordButton* startRecordingBtn = new RecordButton(this);
    startRecordingBtn->setEnabled(false);
    mainLayout->addWidget(startRecordingBtn);
    connect(startReceiverBtn, &QPushButton::toggled, startRecordingBtn, &QPushButton::setEnabled);

    // error label
    statusLabel = new QLabel("", this);
    mainLayout->addWidget(statusLabel);

    // tree widget
    treeWidget = new QTreeWidget(this);
    treeWidget->headerItem()->setHidden(true);
    treeWidget->setRootIsDecorated(false);
    mainLayout->addWidget(treeWidget);

    // create worker and move it to thread
    worker = new DataReceivingWorker(this);
    connect(worker, &DataReceivingWorker::workerStateChanged, [&](QString status){
        statusLabel->setText(status);
    });
    connect(worker, &DataReceivingWorker::onSocketConnected, this, &ReceiverContent::populateTree);
}

ReceiverContent::~ReceiverContent()
{
    worker->pause();
}


void ReceiverContent::onReceiveToggled(bool checked)
{
    if(checked)
    {
        worker->start(portBox->value());
    } else {
        worker->pause();
        clearTreeWidget();
        statusLabel->setText("");
    }
}

void ReceiverContent::populateTree()
{
    QTimer::singleShot(500, [&](){
        // populate props
        auto props = Animations::get()->getProps();
        QHash<QString, QTreeWidgetItem*> propsItemMap;
        foreach(auto prop, props) {
            QTreeWidgetItem* propItem = new QTreeWidgetItem(treeWidget);
            propItem->setText(0, prop["name"].toString());
            propItem->setIcon(0, QIcon(":/resources/cube.png"));
            propsItemMap[prop["id"].toString()] = propItem;
        }

        // actor profiles
        QHash<QString, QTreeWidgetItem*> profilesMap;
        QHash<QString, QTreeWidgetItem*> actorsMap;
        auto actors = Animations::get()->getActors();
        foreach(auto actor, actors) {
            QTreeWidgetItem* profileItem = new QTreeWidgetItem(treeWidget);
            QString profileName = actor["profileName"].toString();
            QString actorName = actor["name"].toString();
            profileItem->setText(0, profileName);
            profileItem->setIcon(0, QIcon(":/resources/icon-rokoko-32.png"));
            profilesMap[profileName] = profileItem;
            QTreeWidgetItem* actorItem = new QTreeWidgetItem(profileItem);
            actorItem->setText(0, actorName);
            actorItem->setIcon(0, QIcon(":/resources/icon-row-suit-32.png"));
            actorsMap[actor["id"].toString()] = actorItem;
        }

        // populate faces
        auto faces = Animations::get()->getFaces();
        foreach(auto face, faces) {
            QTreeWidgetItem* faceItem = nullptr;
            if(face.contains("profileName")) {
                QString faceProfileName = face["profileName"].toString();
                if(profilesMap.contains(faceProfileName))
                    faceItem = new QTreeWidgetItem(profilesMap[faceProfileName]);
            } else
                faceItem = new QTreeWidgetItem(treeWidget);

            faceItem->setText(0, face["faceId"].toString());
            faceItem->setIcon(0, QIcon(":/resources/icon-row-face-32.png"));
            assert(faceItem != nullptr);
        }

        // populate trackers
        auto trackers = Animations::get()->getTrackers();
        foreach(auto tracker, trackers) {
            QString propParentId = tracker["connectionId"].toString();
            QTreeWidgetItem* parentItem = nullptr;
            if(propsItemMap.contains(propParentId))
                parentItem = propsItemMap[propParentId];
            else if(actorsMap.contains(propParentId))
                parentItem = actorsMap[propParentId];
            assert(parentItem != nullptr);

            QTreeWidgetItem* trackerItem = new QTreeWidgetItem(parentItem);
            trackerItem->setText(0, tracker["name"].toString());
            trackerItem->setIcon(0, QIcon(":/resources/icon-vp-32.png"));
        }

        treeWidget->expandAll();
        treeWidget->setItemsExpandable(false);
    });
}

void ReceiverContent::clearTreeWidget()
{
    treeWidget->clear();
}

