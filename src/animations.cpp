#include <iostream>

#include "animations.h"
#include "utils.h"
#include "mapping.h"

#include <QJsonObject>

#include <maya/MObject.h>
#include <maya/MFnTransform.h>
#include <maya/MDagPath.h>

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
    const QMultiMap<QString, MObject> mapping = Mapping::get()->getMapping();

    // apply props animations
    QList<QString> propKeys = mapping.keys();
    for(QString key : propKeys) {
        auto it = mapping.find(key);
        if(it != mapping.end()) {
            while(it != mapping.end()) {
                QString rsId = it.key();
                MObject object = it.value();
                MDagPath dagPath;
                MDagPath::getAPathTo(object, dagPath);

                // convert RS transform to Maya transform
                if(propsMap.contains(rsId)) {
                    QJsonObject propObject = propsMap[rsId];

                    QJsonObject postitionObject = propObject["position"].toObject();
                    QJsonObject rotationObject = propObject["rotation"].toObject();
                    MVector rsPosition(postitionObject["x"].toDouble(),
                                       postitionObject["y"].toDouble(),
                                       postitionObject["z"].toDouble());

                    MQuaternion rsRotation(rotationObject["x"].toDouble(),
                                           rotationObject["y"].toDouble(),
                                           rotationObject["z"].toDouble(),
                                           rotationObject["w"].toDouble());


                    MVector mayaTranslation = Utils::rsToMaya(rsPosition) * sceneScale;
                    MQuaternion mayaQuat = Utils::rsToMaya(rsRotation);

                    // apply converted transform onto mapped object
                    MFnTransform fn(dagPath);
                    fn.setTranslation(mayaTranslation, MSpace::kWorld);
                    fn.setRotation(mayaQuat, MSpace::kWorld);
                } else {
                    // this should never happen
                    std::cout << "MAPPED OBJECTS NOT FOUND IN DATA STREAM!!!!\n";
                }


                ++it;
            }
        }
    }

    // aply tracker animations
    // ...

    // apply faces animations
    // ...

    // apply actors animations
    // ...
}

void _Animations::setSceneScale(float scale)
{
    sceneScale = scale;
}
