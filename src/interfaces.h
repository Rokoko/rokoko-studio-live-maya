#pragma once
#include <QByteArray>
#include <QString>
#include <vector>
#include "framework.h"


class IDataFormatParser
{
public:
    virtual void feed(QByteArray& datagram, qint64 readLen) = 0;
    virtual void parse() = 0;
    virtual void apply() = 0;
    virtual std::vector<FPropSnapshot> propSnapshots() = 0;
    virtual std::vector<FActorSnapshot> actorSnapshots() = 0;
    virtual ~IDataFormatParser() {};
};

