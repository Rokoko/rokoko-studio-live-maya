#include <iostream>

#include "animations.h"
#include "utils.h"
#include "mapping.h"

#include <QJsonObject>
#include <QJsonValue>

#include <maya/MObject.h>
#include <maya/MFnTransform.h>
#include <maya/MDagPath.h>
#include <maya/MItDag.h>

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

    struct Local {
        static void animatePropOrTracker(QJsonObject obj, MDagPath dagPath) {
//            bool isLive = obj["isLive"].toBool();

            // do nothing is not live
//            if(!isLive) return;

            QJsonObject postitionObject = obj["position"].toObject();
            QJsonObject rotationObject = obj["rotation"].toObject();
            MVector rsPosition(postitionObject["x"].toDouble(),
                               postitionObject["y"].toDouble(),
                               postitionObject["z"].toDouble());

            MQuaternion rsRotation(rotationObject["x"].toDouble(),
                                   rotationObject["y"].toDouble(),
                                   rotationObject["z"].toDouble(),
                                   rotationObject["w"].toDouble());


            // convert RS transform to Maya transform
            MVector mayaPosition = Utils::rsToMaya(rsPosition) * Animations::get()->sceneScale();
            MQuaternion mayaRotation = Utils::rsToMaya(rsRotation);

            // apply converted transform onto mapped object
            MFnTransform fn(dagPath);
            fn.setTranslation(mayaPosition, MSpace::kWorld);
            fn.setRotation(mayaRotation, MSpace::kWorld);
        }
    };

    QList<QString> allIds = mapping.keys();
    for(QString rsId : allIds) {
        auto it = mapping.find(rsId);
        if(it != mapping.end()) {
            while(it != mapping.end()) {
                QString rsId = it.key();
                MObject object = it.value();
                MDagPath dagPath;
                MDagPath::getAPathTo(object, dagPath);

                // apply props animations
                if(propsMap.contains(rsId)) {
                    QJsonObject propObject = propsMap[rsId];
                    Local::animatePropOrTracker(propObject, dagPath);
                // apply trackers animations
                } else if(trackersMap.contains(rsId)) {
                    QJsonObject trackerObject = trackersMap[rsId];
                    Local::animatePropOrTracker(trackerObject, dagPath);
                // apply faces animations
                } else if(facesMap.contains(rsId)) {
                // apply actor animations
                } else if(actorsMap.contains(rsId)) {
                    QJsonObject actorObject = actorsMap[rsId];
//                    MItDag it;
//                    it.reset(dagPath, MItDag::kBreadthFirst, MFn::kJoint);
//                    while(!it.isDone()) {
//                        MDagPath jointPath;
//                        it.getPath(jointPath);



//                        it.next();
//                    }

                } else {
                    // this should never happen
                    std::cout << "MAPPED OBJECTS NOT FOUND IN DATA STREAM!!!!\n";
                }


                ++it;
            }
        }
    }
}

void _Animations::setSceneScale(float scale)
{
    _sceneScale = scale;
}
