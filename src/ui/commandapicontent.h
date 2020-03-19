#ifndef COMMANDAPICONTENT_H
#define COMMANDAPICONTENT_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QSpinBox>
#include <QLineEdit>


class CommandApiContent : public QWidget
{
    Q_OBJECT
public:
    CommandApiContent(QWidget* parent=nullptr);
    bool isError(QNetworkReply* reply);
    void ping();
private:
    QNetworkAccessManager* manager=nullptr;
    QSpinBox* portBox=nullptr;
    QLineEdit* ipLineEdit=nullptr;
    QLineEdit* keyLineEdit=nullptr;
    QNetworkRequest makeRequest(QString params);
};

#endif // COMMANDAPICONTENT_H
