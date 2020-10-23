#pragma once

#include "interfaces.h"
#include <QByteArray>
#include <QJsonObject>
#include "lz4frame.h"


class JsonV3Parser : public IDataFormatParser
{
public:
    JsonV3Parser();
    virtual void feed(QByteArray& datagram, qint64 readLen) override;
    virtual void parse() override;
    virtual void apply() override;
    virtual std::vector<FPropSnapshot> propSnapshots() override;
    virtual std::vector<FActorSnapshot> actorSnapshots() override;
private:
    QByteArray mDatagram;
    char* mDecompressionBuffer = nullptr;
    LZ4F_decompressionContext_t mDecompressionContext = nullptr;
    QJsonObject mMessageJson;
};
