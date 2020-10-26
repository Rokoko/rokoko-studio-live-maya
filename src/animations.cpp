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

void _Animations::putProps(const QHash<QString, FPropSnapshot> &props)
{
    propsMap = props;
}

QHash<QString, FPropSnapshot> &_Animations::getProps()
{
    return propsMap;
}

void _Animations::putTrackers(const QHash<QString, FTrackerSnapshot> &trackers)
{
    trackersMap = trackers;
}

QHash<QString, FTrackerSnapshot> &_Animations::getTrackers()
{
    return trackersMap;
}

void _Animations::putActors(const QHash<QString, FActorSnapshot> &actors)
{
    actorsMap = actors;
}

QHash<QString, FActorSnapshot> &_Animations::getActors()
{
    return actorsMap;
}

void _Animations::putFaces(const QHash<QString, FFaceSnapshot> &faces)
{
    facesMap = faces;
}

QHash<QString, FFaceSnapshot> &_Animations::getFaces()
{
    return facesMap;
}

void _Animations::applyAnimationsToMappedObjects()
{
    const QMultiMap<QString, MString> objectMapping = Mapping::get()->getObjectMapping();
    const QMultiMap<QString, MObject> faceObjectMapping = Mapping::get()->getFaceObjectMapping();
    const QHash<QString, QString> mayaToRsBoneNames = Mapping::get()->getBoneMapping();
    const QHash<QString, MQuaternion> studioTPose = Mapping::get()->getStudioTPose();

    QStringList allIds = objectMapping.keys();
    for(const QString &rsId : allIds) {
        auto it = objectMapping.find(rsId);
        // std::cout << "cnt: " << objectMapping.count(rsId) << std::endl;
        if(it != objectMapping.end())
        {
            while(it != objectMapping.end())
            {
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

                    FPropSnapshot propObject = propsMap[rsId];
                    animatePropOrTracker(propObject, dagPath);
                    // printf("prop animated %s - %s\n", dagPath.partialPathName().asChar(), rsId.toStdString().c_str());

                } else if(trackersMap.contains(rsId)) {

                    // apply trackers animations
                    FTrackerSnapshot trackerObject = trackersMap[rsId];
                    animatePropOrTracker(trackerObject, dagPath);

                } else if(actorsMap.contains(rsId)) {
                    // apply actor animations
                    FActorSnapshot actorObject = actorsMap[rsId];

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
                            QVector3D rsBonePos = actorObject.body[rsBoneName].position;
                            QQuaternion rsBoneQuat = actorObject.body[rsBoneName].rotation;

                            MQuaternion studioTposeRot = MQuaternion::identity;
                            if(studioTPose.contains(rsBoneName))
                            {
                                // convert studio T pose rotation into maya space
                                studioTposeRot = Utils::rsToMaya(studioTPose[rsBoneName]);
                            }

                            // convert rs transform to maya transform
                            MVector boneLocation = Utils::rsToMaya(MVector(rsBonePos.x(),
                                                                           rsBonePos.y(),
                                                                           rsBonePos.z()) * sceneScale());
                            boneLocation = boneLocation.rotateBy(referenceQuat);
                            boneLocation += referenceOffset;
                            // convert studio rotation into maya space
                            MQuaternion boneQuat = Utils::rsToMaya(MQuaternion(rsBoneQuat.x(),
                                                                   rsBoneQuat.y(),
                                                                   rsBoneQuat.z(),
                                                                   rsBoneQuat.scalar()));

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

    QStringList faceIds = faceObjectMapping.keys();
    for(QString &rsId : faceIds) {
        auto faceIt = faceObjectMapping.find(rsId);
        if(faceIt != faceObjectMapping.end()) {
            while(faceIt != faceObjectMapping.end())
            {
                if(facesMap.contains(rsId)) {
                    // apply face animations

                    MObject object = faceIt.value();

//                    if(object.isNull()) {
//                        stopReceiverCallback();
//                        Mapping::get()->syncMapping();
//                        break;
//                    }

                    const QStringList faceShapeNames = Mapping::get()->getFaceShapeNames();
                    // get input data for this face
                    FFaceSnapshot faceObject = facesMap[rsId];

                    // access mapped blendshape node
                    MFnBlendShapeDeformer faceFn(object);

                    // TODO: run this once when receiver stared
                    // prepare weight weight name to attribute plug map
                    QHash <QString, MPlug> weightsMap;
                    Utils::fillFaceWeightsMap(faceFn, weightsMap);

                    // for each studio shape name set weight value
                    for(QString studioShapeName : faceShapeNames) {
                        double weight = faceObject.shapes[studioShapeName] * 0.01;

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

                }
                ++faceIt;
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

void _Animations::animatePropOrTracker(FPropSnapshot obj, const MDagPath &dagPath)
{
    MVector rsPosition(obj.position.x(),
                       obj.position.y(),
                       obj.position.z());

    MQuaternion rsRotation(obj.rotation.x(),
                           obj.rotation.y(),
                           obj.rotation.z(),
                           obj.rotation.scalar());

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

void _Animations::animatePropOrTracker(FTrackerSnapshot obj, const MDagPath &dagPath)
{
    FPropSnapshot pSnapshot;
    pSnapshot.position = obj.position;
    pSnapshot.rotation = obj.rotation;
    animatePropOrTracker(pSnapshot, dagPath);
}





