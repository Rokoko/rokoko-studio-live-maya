#include <iostream>

#include "receivercontent.h"
#include "receiverworker.h"
#include "ui/button.h"
#include "ui/recordbutton.h"

#include <QThread>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QCheckBox>


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

    // test label
    testLabel = new QLabel("...", this);
    mainLayout->addWidget(testLabel);

    // create worker and move it to thread
    worker = new DataReceivingWorker(this);
    listenerThread = new QThread(this);
    listenerThread->setObjectName("RSListenerThread");
    worker->moveToThread(listenerThread);
    connect(worker, &DataReceivingWorker::frameReceived, [this](QJsonObject obj) {
        QString t = QString("%1").arg(obj["playbackTimestamp"].toDouble());
        testLabel->setText(t);
    });
    connect(worker, &DataReceivingWorker::onError, [this](QString err){
        testLabel->setText(err);
    });
    listenerThread->start();
}

ReceiverContent::~ReceiverContent()
{
    worker->pause();
    listenerThread->terminate();
    listenerThread->wait();
}

void ReceiverContent::handleListenerError(QString err)
{
    testLabel->setText(err);
}

void ReceiverContent::onReceiveToggled(bool checked)
{
    if(checked) {
        worker->start();
    } else {
        worker->pause();
    }
}
