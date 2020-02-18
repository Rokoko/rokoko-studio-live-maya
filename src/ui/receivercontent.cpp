#include "receivercontent.h"
#include "ui/button.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>


ReceiverContent::ReceiverContent(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(1);

    // port field
    QHBoxLayout* portLayout = new QHBoxLayout(this);
    QLabel* portLabel = new QLabel("Port:", this);
    QSpinBox* portBox = new QSpinBox(this);
    portBox->setAlignment(Qt::AlignCenter);
    portBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    portBox->setMaximum(65535);
    portBox->setValue(14043);

    portLayout->addWidget(portLabel);
    portLayout->addWidget(portBox);
    mainLayout->addLayout(portLayout);

    // scene scale field
    QHBoxLayout* sceneScaleLayout = new QHBoxLayout(this);
    QLabel* sceneScaleLabel = new QLabel("Scene scale:", this);
    QDoubleSpinBox* sceneScaleBox = new QDoubleSpinBox(this);
    sceneScaleBox->setAlignment(Qt::AlignCenter);
    sceneScaleBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    sceneScaleBox->setValue(1.0);
    sceneScaleLayout->addWidget(sceneScaleLabel);
    sceneScaleLayout->addWidget(sceneScaleBox);
    mainLayout->addLayout(sceneScaleLayout);

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
    startReceiverBtn->setDrawIcon(true);
    mainLayout->addWidget(startReceiverBtn);

}
