#include "commandapicontent.h"
#include "constants.h"
#include "ui/button.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QSpinBox>


CommandApiContent::CommandApiContent(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->setSpacing(1);

    // address
    QLineEdit* addrLineEdit = new QLineEdit("127.0.0.1", this);
    QString oIpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp oIpRegex ("^" + oIpRange
                  + "\\." + oIpRange
                  + "\\." + oIpRange
                  + "\\." + oIpRange + "$");
    QRegExpValidator* ipValidator = new QRegExpValidator(oIpRegex, addrLineEdit);
    addrLineEdit->setValidator(ipValidator);
    // addrLineEdit->setInputMask("000.000.000.000");
    formLayout->addRow("Address:", addrLineEdit);

    // port
    QSpinBox* portBox = new QSpinBox(this);
    portBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    portBox->setMaximum(65535);
    portBox->setValue(DEFAULT_RS_PORT);
    portBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    formLayout->addRow("Port:", portBox);

    // key
    QLineEdit* keyLineEdit = new QLineEdit("1234", this);
    formLayout->addRow("Key:", keyLineEdit);

    mainLayout->addLayout(formLayout);

    // command buttons
    QHBoxLayout* commandsLayout = new QHBoxLayout(this);
    // calibrate
    ButtonParams startCalibrationParams;
    startCalibrationParams.uncheckedStateText = "";
    startCalibrationParams.checkedStateText = "";
    startCalibrationParams.uncheckedIconPath = ":/resources/icon-straight-pose-32.png";
    startCalibrationParams.iconAlignment = Qt::AlignCenter;
    Button* startCalibrationBtn = new Button(this, startCalibrationParams);
    commandsLayout->addWidget(startCalibrationBtn);
    // restart
    ButtonParams restartParams;
    restartParams.uncheckedStateText = "";
    restartParams.checkedStateText = "";
    restartParams.uncheckedIconPath = ":/resources/icon-restart-32.png";
    restartParams.iconAlignment = Qt::AlignCenter;
    Button* restartBtn = new Button(this, restartParams);
    commandsLayout->addWidget(restartBtn);
    // record
    ButtonParams recordParams;
    recordParams.uncheckedStateText = "";
    recordParams.checkedStateText = "";
    recordParams.uncheckedIconPath = ":/resources/icon-record-32.png";
    recordParams.iconAlignment = Qt::AlignCenter;
    Button* recordBtn = new Button(this, recordParams);
    commandsLayout->addWidget(recordBtn);
    // stop
    ButtonParams stopParams;
    stopParams.uncheckedStateText = "";
    stopParams.checkedStateText = "";
    stopParams.uncheckedIconPath = ":/resources/icon-stop-white-32.png";
    stopParams.iconAlignment = Qt::AlignCenter;
    Button* stopBtn = new Button(this, stopParams);
    commandsLayout->addWidget(stopBtn);

    mainLayout->addLayout(commandsLayout);
}
