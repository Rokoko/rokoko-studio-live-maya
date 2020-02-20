#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "singleton.h"
#include <QHash>
#include <QJsonObject>

class _Animations
{
public:
    _Animations();
    void putProps(const QHash<QString, QJsonObject>&);
    QHash<QString, QJsonObject> &getProps();
    void putTrackers(const QHash<QString, QJsonObject>&);
    QHash<QString, QJsonObject> &getTrackers();
private:
    // prop id - json
    QHash<QString, QJsonObject> propsMap;
    // tracker name - json
    QHash<QString, QJsonObject> trackersMap;
};

typedef Singleton<_Animations> Animations;

#endif // ANIMATIONS_H
