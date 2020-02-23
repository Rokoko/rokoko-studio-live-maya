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
    void putActors(const QHash<QString, QJsonObject>&);
    QHash<QString, QJsonObject> &getActors();
    void putFaces(const QHash<QString, QJsonObject>&);
    QHash<QString, QJsonObject> &getFaces();

    void applyAnimationsToMappedObjects();

    float timestamp;
    float playbackTimestamp;
private:
    // prop id - json
    QHash<QString, QJsonObject> propsMap;
    // tracker name - json
    QHash<QString, QJsonObject> trackersMap;
    // actor id - json
    QHash<QString, QJsonObject> actorsMap;
    // faceId - json
    QHash<QString, QJsonObject> facesMap;
};

typedef Singleton<_Animations> Animations;

#endif // ANIMATIONS_H

