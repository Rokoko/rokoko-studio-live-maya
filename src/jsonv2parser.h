#pragma once

#include "interfaces.h"
#include <QByteArray>
#include <QJsonObject>


class JsonV2Parser : public IDataFormatParser
{
public:
    JsonV2Parser();
    virtual void feed(QByteArray& datagram, qint64 readLen) override;
    virtual void parse() override;
    virtual void apply() override;
    virtual std::vector<FPropSnapshot> propSnapshots() override;
    virtual std::vector<FActorSnapshot> actorSnapshots() override;
private:
    QByteArray mDatagram;
    QJsonObject mMessageJson;
};
