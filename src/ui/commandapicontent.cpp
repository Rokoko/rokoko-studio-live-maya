#include "commandapicontent.h"
#include "constants.h"
#include "utils.h"
#include "ui/button.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QSpinBox>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>



CommandApiContent::CommandApiContent(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->setSpacing(1);

    // address
    ipLineEdit = new QLineEdit("127.0.0.1", this);
    ipLineEdit->setAlignment(Qt::AlignCenter);
    QString oIpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp oIpRegex ("^" + oIpRange
                      + "\\." + oIpRange
                      + "\\." + oIpRange
                      + "\\." + oIpRange + "$");
    QRegExpValidator* ipValidator = new QRegExpValidator(oIpRegex, ipLineEdit);
    ipLineEdit->setValidator(ipValidator);
    formLayout->addRow("Address:", ipLineEdit);

    // port
    portBox = new QSpinBox(this);
    portBox->setAlignment(Qt::AlignCenter);
    portBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    portBox->setMaximum(65535);
    portBox->setValue(DEFAULT_RS_API_PORT);
    portBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    formLayout->addRow("Port:", portBox);

    // key
    keyLineEdit = new QLineEdit("1234", this);
    keyLineEdit->setAlignment(Qt::AlignCenter);
    formLayout->addRow("Key:", keyLineEdit);

    mainLayout->addLayout(formLayout);

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, [&](QNetworkReply* reply) {
        if(!isError(reply))
            Utils::mayaPrintMessage("Success");
    });

    // command buttons
    QHBoxLayout* commandsLayout = new QHBoxLayout(this);
    commandsLayout->setContentsMargins(1, 1, 1, 1);
    commandsLayout->setSpacing(1);
    // calibrate
    ButtonParams startCalibrationParams;
    startCalibrationParams.uncheckedStateText = "";
    startCalibrationParams.checkedStateText = "";
    startCalibrationParams.uncheckedIconPath = ":/resources/icon-straight-pose-32.png";
    startCalibrationParams.iconAlignment = Qt::AlignCenter;
    Button* startCalibrationBtn = new Button(this, startCalibrationParams);
    connect(startCalibrationBtn, &QPushButton::clicked, [&]() {
        QByteArray params;
        // optional params
        //params.append("smartsuit_name=BG2&");
        //params.append("countdown_delay=3");
        manager->post(makeRequest("calibrate"), params);
    });

    commandsLayout->addWidget(startCalibrationBtn);
    // restart
    ButtonParams restartParams;
    restartParams.uncheckedStateText = "";
    restartParams.checkedStateText = "";
    restartParams.uncheckedIconPath = ":/resources/icon-restart-32.png";
    restartParams.iconAlignment = Qt::AlignCenter;
    Button* restartBtn = new Button(this, restartParams);
    connect(restartBtn, &QPushButton::clicked, [&]() {
        QByteArray params;
//        params.append("smartsuit_name=BG2");
        manager->post(makeRequest("restart"), params);
    });
    commandsLayout->addWidget(restartBtn);
    // record
    ButtonParams recordParams;
    recordParams.uncheckedStateText = "";
    recordParams.checkedStateText = "";
    recordParams.uncheckedIconPath = ":/resources/icon-record-32.png";
    recordParams.iconAlignment = Qt::AlignCenter;
    Button* recordBtn = new Button(this, recordParams);
    connect(recordBtn, &QPushButton::clicked, [&]() {
        QByteArray params;
//        params.append("filename=test");
        manager->post(makeRequest("recording/start"), params);
    });
    commandsLayout->addWidget(recordBtn);
    // stop
    ButtonParams stopParams;
    stopParams.uncheckedStateText = "";
    stopParams.checkedStateText = "";
    stopParams.uncheckedIconPath = ":/resources/icon-stop-white-32.png";
    stopParams.iconAlignment = Qt::AlignCenter;
    Button* stopBtn = new Button(this, stopParams);
    connect(stopBtn, &QPushButton::clicked, [&]() {
        QByteArray params;
        manager->post(makeRequest("recording/stop"), params);
    });
    commandsLayout->addWidget(stopBtn);

    mainLayout->addLayout(commandsLayout);
}

bool CommandApiContent::isError(QNetworkReply* reply)
{

    if(reply->error() == QNetworkReply::NetworkError::ConnectionRefusedError) {
        // command api server disabled completely
        Utils::spawnMayaError("Couldn't conect to Rokoko Studio!");
        return true;
    } else {
        QByteArray result = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(result);
        QJsonObject resultObject = doc.object();

        QString responseMessage = resultObject["response_code"].toString();
        QString descriptionMessage = resultObject["description"].toString();

        // command api server enabled but something is wrong
        if(responseMessage != "OK")
        {
            if(responseMessage == "INVALID_REQUEST") {
                Utils::spawnMayaError(QString("%1: %2 %3")
                              .arg(responseMessage)
                              .arg(descriptionMessage)
                              .arg("Check your API key"));
            } else {
                Utils::spawnMayaError(QString("%1: %2")
                              .arg(responseMessage)
                              .arg(descriptionMessage));
            }
            return true;
        }
        // everything is ok
        return false;
    }
}

void CommandApiContent::ping()
{
    manager->get(makeRequest(""));
}

QNetworkRequest CommandApiContent::makeRequest(QString params)
{
    return QNetworkRequest(QUrl(QString("http://%1:%2/v1/%3/%4")
                                .arg(ipLineEdit->text())
                                .arg(portBox->value())
                                .arg(keyLineEdit->text())
                                .arg(params)));
}
