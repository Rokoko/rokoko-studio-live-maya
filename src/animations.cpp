#include "animations.h"

_Animations::_Animations()
{

}

void _Animations::putProps(const QHash<QString, QJsonObject> &props)
{
    propsMap = props;
}

QHash<QString, QJsonObject> &_Animations::getProps()
{
    return propsMap;
}

void _Animations::putTrackers(const QHash<QString, QJsonObject> &trackers)
{
    trackersMap = trackers;
}

QHash<QString, QJsonObject> &_Animations::getTrackers()
{
    return trackersMap;
}

void _Animations::putActors(const QHash<QString, QJsonObject> &actors)
{
    actorsMap = actors;
}

QHash<QString, QJsonObject> &_Animations::getActors()
{
    return actorsMap;
}

void _Animations::putFaces(const QHash<QString, QJsonObject> &faces)
{
    facesMap = faces;
}

QHash<QString, QJsonObject> &_Animations::getFaces()
{
    return facesMap;
}

void _Animations::applyAnimationsToMappedObjects()
{

}
