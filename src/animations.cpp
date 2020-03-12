#include <iostream>

#include "animations.h"
#include "recorder.h"
#include "utils.h"
#include "mapping.h"

#include <QJsonObject>
#include <QJsonValue>

#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MEulerRotation.h>
#include <maya/MObjectArray.h>
#include <maya/MFnTransform.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MDagPath.h>
#include <maya/MItDag.h>
#include <maya/MPlug.h>
#include <maya/MMatrix.h>


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
    const QMultiMap<QString, MString> objectMapping = Mapping::get()->getObjectMapping();
    const QHash<QString, QString> mayaToRsBoneNames = Mapping::get()->getBoneMapping();
    const QHash<QString, MQuaternion> studioTPose = Mapping::get()->getStudioTPose();

    QList<QString> allIds = objectMapping.keys();
    for(const QString &rsId : allIds) {
        auto it = objectMapping.find(rsId);
        // std::cout << "cnt: " << objectMapping.count(rsId) << std::endl;
        if(it != objectMapping.end()) {
            while(it != objectMapping.end())
            {
                QString rsId = it.key();
                MString objectPathString = it.value();

                MSelectionList ls;
                ls.add(objectPathString);
                MDagPath objectPath;
                MStatus found = ls.getDagPath(0, objectPath);

                if(found != MStatus::kSuccess) {
                    // node was removed during iteration
                    stopReceiverCallback();
                    Mapping::get()->syncMapping();
                    break;
                }

                MObject object = objectPath.node();

                MDagPath dagPath;
                MDagPath::getAPathTo(object, dagPath);

                if(propsMap.contains(rsId)) {
                    // apply props animations

                    QJsonObject propObject = propsMap[rsId];
                    animatePropOrTracker(propObject, dagPath);
                    // printf("prop animated %s - %s\n", dagPath.partialPathName().asChar(), rsId.toStdString().c_str());

                } else if(trackersMap.contains(rsId)) {

                    // apply trackers animations
                    QJsonObject trackerObject = trackersMap[rsId];
                    animatePropOrTracker(trackerObject, dagPath);

                } else if(facesMap.contains(rsId)) {
                    // apply face animations

                    const QStringList faceShapeNames = Mapping::get()->getFaceShapeNames();
                    // get input data for this face
                    QJsonObject faceObject = facesMap[rsId];

                    // access mapped blendshape node
                    MFnBlendShapeDeformer faceFn(object);

                    // TODO: run this once when receiver stared
                    // prepare weight weight name to attribute plug map
                    QHash <QString, MPlug> weightsMap;
                    Utils::fillFaceWeightsMap(faceFn, weightsMap);

                    // for each studio shape name set weight value
                    for(QString studioShapeName : faceShapeNames) {
                        double weight = faceObject[studioShapeName].toDouble() * 0.01;

                        MString bsFieldName = BLEND_SHAPE_PREFIX + studioShapeName.toStdString().c_str();
                        MStatus fieldPlugStatus;
                        MPlug weightAttr = faceFn.findPlug(bsFieldName, true, &fieldPlugStatus);
                        if(fieldPlugStatus == MStatus::kSuccess) {
                            MString mappedName;
                            weightAttr.getValue(mappedName);
                            if(weightsMap.contains(mappedName.asChar())) {
                                MPlug weightPlug = weightsMap[mappedName.asChar()];
                                weightPlug.setDouble(weight);
                                if(recordingEnabled) {

                                    // this functions will be called later
                                    Recorder::get()->recordFace(timestamp, [object, weight, weightPlug](int frame) {
                                        Recorder::get()->keyframeNumericAttribute(frame, weightPlug, weight);
                                    });
                                }
                            }
                        }
                    }

                } else if(actorsMap.contains(rsId)) {
                    // apply actor animations
                    QJsonObject actorObject = actorsMap[rsId];

                    // Hips joint mapped implicitly for user
                    // BFS starting from Hips and apply data to each joints

                    // We transform HIK source skeleton. To see results on custom character user should create
                    // Another character and define it, than set it as target for our generated character.

                    MItDag jIt;
                    MFnDagNode hipDagNode(dagPath);
                    MVector referenceOffset;
                    MQuaternion referenceQuat;
                    if(hipDagNode.parentCount() > 0) {
                        MObject hipsParentObject = hipDagNode.parent(0);
                        MDagPath path;
                        MDagPath::getAPathTo(hipsParentObject, path);
                        MFnTransform parentFnTransform(path);
                        referenceOffset = parentFnTransform.getTranslation(MSpace::kWorld);
                        parentFnTransform.getRotation(referenceQuat, MSpace::kWorld);
                        referenceQuat.normalizeIt();
                    }

                    jIt.reset(dagPath, MItDag::kBreadthFirst, MFn::kJoint);
                    while(!jIt.isDone()) {
                        MDagPath jointPath;
                        jIt.getPath(jointPath);

                        QString jointPathString(jointPath.fullPathName().asChar());
                        // this name contains character name CHARNAME_BONENAME
                        QString jointName = jointPathString.split("|", QString::SkipEmptyParts).takeLast();
                        QString mayaBoneName = jointName.split("_").takeLast();
                        // find rs bone based on joint name
                        if(mayaToRsBoneNames.contains(mayaBoneName)) {
                            QString rsBoneName = mayaToRsBoneNames[mayaBoneName];
                            QJsonObject rsBoneObject = actorObject[rsBoneName].toObject();
                            QJsonObject rsBonePosObject = rsBoneObject["position"].toObject();
                            QJsonObject rsBoneQuatObject = rsBoneObject["rotation"].toObject();

                            MQuaternion studioTposeRot = MQuaternion::identity;
                            if(studioTPose.contains(rsBoneName))
                            {
                                // convert studio T pose rotation into maya space
                                studioTposeRot = Utils::rsToMaya(studioTPose[rsBoneName]);
                            }

                            // convert rs transform to maya transform
                            MVector boneLocation = Utils::rsToMaya(MVector(rsBonePosObject["x"].toDouble(),
                                                                           rsBonePosObject["y"].toDouble(),
                                                                           rsBonePosObject["z"].toDouble()) * sceneScale());
                            boneLocation = boneLocation.rotateBy(referenceQuat);
                            boneLocation += referenceOffset;
                            // convert studio rotation into maya space
                            MQuaternion boneQuat = Utils::rsToMaya(MQuaternion(rsBoneQuatObject["x"].toDouble(),
                                                                   rsBoneQuatObject["y"].toDouble(),
                                                                   rsBoneQuatObject["z"].toDouble(),
                                                                   rsBoneQuatObject["w"].toDouble()));

                            MFnIkJoint fnTr(jointPath);
                            fnTr.setTranslation(boneLocation, MSpace::kWorld);

                            // calc final rotation
                            boneQuat = studioTposeRot.inverse() * boneQuat;
                            boneQuat *= referenceQuat;
                            boneQuat.normalizeIt();
                            fnTr.setRotation(boneQuat, MSpace::kWorld);

                            if(recordingEnabled) {
                                MVector recordLocation = fnTr.getTranslation(MSpace::kTransform);
                                MQuaternion recordRotation;
                                fnTr.getRotation(recordRotation);
                                Recorder::get()->recordBone(timestamp, [recordLocation, jointPath, recordRotation](int frame) {
                                    Recorder::get()->keyframeNumericAttribute("tx", frame, jointPath, recordLocation.x);
                                    Recorder::get()->keyframeNumericAttribute("ty", frame, jointPath, recordLocation.y);
                                    Recorder::get()->keyframeNumericAttribute("tz", frame, jointPath, recordLocation.z);

                                    MEulerRotation euAngle = recordRotation.asEulerRotation();
                                    Recorder::get()->keyframeNumericAttribute("rx", frame, jointPath, euAngle.x);
                                    Recorder::get()->keyframeNumericAttribute("ry", frame, jointPath, euAngle.y);
                                    Recorder::get()->keyframeNumericAttribute("rz", frame, jointPath, euAngle.z);
                                });
                            }
                        }

                        jIt.next();
                    }

                } else {
                    // this block executes only if rsId is not found in mapped objects
                    // for example user opened maya scene that doesn't corresponds to opened studio project
                    // TODO: tell user about problematic objects

//                    std::cout << "MAPPED OBJECTS NOT FOUND IN DATA STREAM!!!!\n";
                }

                ++it;
            }
        }
    }
}

void _Animations::recordingToggled(bool enabled)
{
    recordingEnabled = enabled;
    Recorder::get()->recordingToggled(enabled);
    if(!recordingEnabled) {
        // put cached data into timeline
        Recorder::get()->finalizeRecording();
    }

}

void _Animations::setSceneScale(float scale)
{
    _sceneScale = scale;
}

void _Animations::reset()
{
    propsMap.clear();
    trackersMap.clear();
    actorsMap.clear();
    facesMap.clear();
}

void _Animations::animatePropOrTracker(QJsonObject obj, const MDagPath &dagPath)
{
//    bool isLive = obj["isLive"].toBool();

     // do nothing if not live
//    if(!isLive) return;

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
    MTransformationMatrix finalTransform(MTransformationMatrix::identity);
    finalTransform.setTranslation(mayaPosition, MSpace::kWorld);
    finalTransform.setRotationQuaternion(mayaRotation.x, mayaRotation.y, mayaRotation.z, mayaRotation.w);

    // apply converted transform onto mapped object
    MFnTransform fn(dagPath);
    fn.set(finalTransform);

    if(recordingEnabled) {
        Recorder::get()->recordPropOrTracker(timestamp, [mayaPosition, mayaRotation, dagPath](int frame){
            Recorder::get()->keyframeNumericAttribute("tx", frame, dagPath, mayaPosition.x);
            Recorder::get()->keyframeNumericAttribute("ty", frame, dagPath, mayaPosition.y);
            Recorder::get()->keyframeNumericAttribute("tz", frame, dagPath, mayaPosition.z);

            MEulerRotation euAngle = mayaRotation.asEulerRotation();
            Recorder::get()->keyframeNumericAttribute("rx", frame, dagPath, euAngle.x);
            Recorder::get()->keyframeNumericAttribute("ry", frame, dagPath, euAngle.y);
            Recorder::get()->keyframeNumericAttribute("rz", frame, dagPath, euAngle.z);
        });
    }
}





