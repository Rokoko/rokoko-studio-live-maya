#include <iostream>
#include "mapping.h"
#include "utils.h"
#include <QFile>
#include <QTimer>
#include <maya/MFn.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItSelectionList.h>
#include <maya/MMessage.h>
#include <maya/MSceneMessage.h>
#include <maya/MAngle.h>


const QString MAPPING_FILED_NAME = "RokokoMapping";



_Mapping::_Mapping()
{
    // torso
    boneMapping["Hips"] = "hip";
    boneMapping["Spine"] = "spine";
    boneMapping["Spine1"] = "chest";
    boneMapping["Neck"] = "neck";
    boneMapping["Head"] = "head";

    // left hand
    boneMapping["LeftShoulder"] = "leftShoulder";
    boneMapping["LeftArm"] = "leftUpperArm";
    boneMapping["LeftForeArm"] = "leftLowerArm";
    boneMapping["LeftHand"] = "leftHand";

    /*boneMapping["LeftHandThumb1"] = "leftThumbProximal";
    boneMapping["LeftHandThumb2"] = "leftThumbMedial";
    boneMapping["LeftHandThumb3"] = "leftThumbDistal";
    boneMapping["LeftHandThumb4"] = "leftThumbTip";

    boneMapping["LeftHandIndex1"] = "leftIndexProximal";
    boneMapping["LeftHandIndex2"] = "leftIndexMedial";
    boneMapping["LeftHandIndex3"] = "leftIndexDistal";
    boneMapping["LeftHandIndex4"] = "leftIndexTip";

    boneMapping["LeftHandMiddle1"] = "leftMiddleProximal";
    boneMapping["LeftHandMiddle2"] = "leftMiddleMedial";
    boneMapping["LeftHandMiddle3"] = "leftMiddleDistal";
    boneMapping["LeftHandMiddle4"] = "leftMiddleTip";

    boneMapping["LeftHandRing1"] = "leftRingProximal";
    boneMapping["LeftHandRing2"] = "leftRingMedial";
    boneMapping["LeftHandRing3"] = "leftRingDistal";
    boneMapping["LeftHandRing4"] = "leftRingTip";

    boneMapping["LeftHandPinky1"] = "leftLittleProximal";
    boneMapping["LeftHandPinky2"] = "leftLittleMedial";
    boneMapping["LeftHandPinky3"] = "leftLittleDistal";
    boneMapping["LeftHandPinky4"] = "leftLittleTip";*/


    // right hand
    boneMapping["RightShoulder"] = "rightShoulder";
    boneMapping["RightArm"] = "rightUpperArm";
    boneMapping["RightForeArm"] = "rightLowerArm";
    boneMapping["RightHand"] = "rightHand";

    /*boneMapping["RightHandThumb1"] = "rightThumbProximal";
    boneMapping["RightHandThumb2"] = "rightThumbMedial";
    boneMapping["RightHandThumb3"] = "rightThumbDistal";
    boneMapping["RightHandThumb4"] = "rightThumbTip";

    boneMapping["RightHandIndex1"] = "rightIndexProximal";
    boneMapping["RightHandIndex2"] = "rightIndexMedial";
    boneMapping["RightHandIndex3"] = "rightIndexDistal";
    boneMapping["RightHandIndex4"] = "rightIndexTip";

    boneMapping["RightHandMiddle1"] = "rightMiddleProximal";
    boneMapping["RightHandMiddle2"] = "rightMiddleMedial";
    boneMapping["RightHandMiddle3"] = "rightMiddleDistal";
    boneMapping["RightHandMiddle4"] = "rightMiddleTip";

    boneMapping["RightHandRing1"] = "rightRingProximal";
    boneMapping["RightHandRing2"] = "rightRingMedial";
    boneMapping["RightHandRing3"] = "rightRingDistal";
    boneMapping["RightHandRing4"] = "rightRingTip";

    boneMapping["RightHandPinky1"] = "rightLittleProximal";
    boneMapping["RightHandPinky2"] = "rightLittleMedial";
    boneMapping["RightHandPinky3"] = "rightLittleDistal";
    boneMapping["RightHandPinky4"] = "rightLittleTip";*/

    // left leg
    boneMapping["LeftUpLeg"] = "leftUpLeg";
    boneMapping["LeftLeg"] = "leftLeg";
    boneMapping["LeftFoot"] = "leftFoot";
    boneMapping["LeftToeBase"] = "leftToe";
    //    boneMapping["??"] = "leftToeEnd";

    // right leg
    boneMapping["RightUpLeg"] = "rightUpLeg";
    boneMapping["RightLeg"] = "rightLeg";
    boneMapping["RightFoot"] = "rightFoot";
    boneMapping["RightToeBase"] = "rightToe";
    //    boneMapping["??"] = "rightToeEnd";


    // TPose
    studioTPose["hip"] = MQuaternion(0.00, 0.00, 0.00, 1.00);
    studioTPose["spine"] = MQuaternion(-0.00, 0.00, -1.00, 0.00);
    studioTPose["chest"] = MQuaternion(-0.00, -0.00, 1.00, 0.00);
    studioTPose["neck"] = MQuaternion(-0.00, 0.00, -1.00, 0.00);
    studioTPose["head"] = MQuaternion(-0.00, -0.00, 1.00, 0.00);

    studioTPose["leftShoulder"] = MQuaternion(0.000, 0.000, -0.707, 0.707);
    studioTPose["leftUpperArm"] = MQuaternion(0.500, 0.500, -0.500, 0.500);
    studioTPose["leftLowerArm"] = MQuaternion(0.500, 0.500, -0.500, 0.500);
    studioTPose["leftHand"] = MQuaternion(0.500, 0.500, -0.500, 0.500);

    studioTPose["rightShoulder"] = MQuaternion(0.000, -0.000, 0.707, 0.707);
    studioTPose["rightUpperArm"] = MQuaternion(0.500, -0.500, 0.500, 0.500);
    studioTPose["rightLowerArm"] = MQuaternion(0.500, -0.500, 0.500, 0.500);
    studioTPose["rightHand"] = MQuaternion(0.500, -0.500, 0.500, 0.500);

    studioTPose["leftUpLeg"] = MQuaternion(-0.000, 0.707, 0.000, 0.707);
    studioTPose["leftLeg"] = MQuaternion(-0.000, 0.707, 0.000, 0.707);
    studioTPose["leftFoot"] = MQuaternion(-0.000, 0.707, -0.707, 0.000);
    studioTPose["leftToe"] = MQuaternion(-0.000, 0.707, -0.707, 0.000);

    studioTPose["rightUpLeg"] = MQuaternion(-0.000, -0.707, -0.000, 0.707);
    studioTPose["rightLeg"] = MQuaternion(-0.000, -0.707, -0.000, 0.707);
    studioTPose["rightFoot"] = MQuaternion(-0.000, 0.707, -0.707, 0.000);
    studioTPose["rightToe"] = MQuaternion(0.000, 0.707, -0.707, 0.000);

    // register callbacks
    MCallbackId beforeNewId = MSceneMessage::addCheckCallback(MSceneMessage::kBeforeNewCheck, [](bool* recCode, void* clientData) {
        Mapping::get()->clear();
    });
    MCallbackId beforeOpenId = MSceneMessage::addCheckCallback(MSceneMessage::kBeforeOpenCheck, [](bool* recCode, void* clientData) {
        Mapping::get()->clear();
    });
    callbacks.append(beforeNewId);
    callbacks.append(beforeOpenId);

}

void _Mapping::mapRSObjectToSelection(QString rsObjectID)
{
    QFile cmdFile(":/resources/mapToSelectedObjects.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME);
    cmdString.replace("RS_ID_TAG", rsObjectID);
    MGlobal::executeCommand(cmdString.toStdString().c_str());

    // put ids into objectsMap
    MSelectionList ls;
    MGlobal::getActiveSelectionList(ls);
    MItSelectionList iter(ls);
    while(!iter.isDone()) {
        MObject object;
        MStatus dpNodeStatus = iter.getDependNode(object);
        MFnDependencyNode node(object);
        if(!objectsMap.contains(rsObjectID, object))
        {
            objectsMap.insert(rsObjectID, object);
            std::cout << "Store: " << node.name().asChar() << "\n";
        } else {
            std::cout << "Already mapped: " << node.name().asChar() << "\n";
        }
        iter.next();
    }

    auto objects = objectsMap.values(rsObjectID);
    for(auto obj : objects) {
        MFnDependencyNode node(obj);
        std::cout << node.name() << " < ";
    }
    std::cout << "\n";
}

void _Mapping::unmapRSObject(QString rsObjectID, bool selected=false)
{
    QFile cmdFile(":/resources/unmapRSObject.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME);
    cmdString.replace("RS_ID_TAG", rsObjectID);
    cmdString.replace("SELECTED_ONLY", selected ? "true" : "false");
    MGlobal::executeCommand(cmdString.toStdString().c_str());

    // remove objects from mapping
    if(selected) {
        MSelectionList ls;
        MGlobal::getActiveSelectionList(ls);
        MItSelectionList iter(ls);
        while(!iter.isDone()) {
            MObject object;
            MStatus dpNodeStatus = iter.getDependNode(object);
            if(dpNodeStatus != MStatus::kSuccess) {
                std::cout << "FAILED TO FETCH MObject\n";
            }
            MFnDependencyNode node(object);

            if(objectsMap.contains(rsObjectID, object)) {
                std::cout << "FOUND\n";
                objectsMap.remove(rsObjectID, object);
            } else {
                std::cout << "NOT FOUND\n";
            }

            iter.next();
        }
    } else {
        // remove all maya objects associated with this rsId
        if(objectsMap.contains(rsObjectID)) {
            objectsMap.remove(rsObjectID);
        }
    }

    auto objects = objectsMap.values(rsObjectID);
    for(auto obj : objects) {
        MFnDependencyNode node(obj);
        std::cout << node.name() << " < ";
    }
    std::cout << "\n";
}

void _Mapping::selectObjects(QString rsObjectID)
{
    QFile cmdFile(":/resources/selectMappedObjects.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME);
    cmdString.replace("RS_ID_TAG", rsObjectID);
    MGlobal::executeCommand(cmdString.toStdString().c_str());
}

void _Mapping::syncMapping()
{
    // erase all
    objectsMap.clear();

    // iterate over all transform nodes
    MItDependencyNodes nodesIt(MFn::kTransform);
    while(!nodesIt.isDone()) {
        MObject object = nodesIt.item();
        MFnDependencyNode fn(object);
        MString fieldName(MAPPING_FILED_NAME.toStdString().c_str());
        bool attrFound = fn.hasAttribute(fieldName);
        if(attrFound)
        {
            MString rsIdValue = fn.findPlug(fieldName).asString();
            objectsMap.insert(rsIdValue.asChar(), object);
            std::cout << "sync object: " << rsIdValue.asChar() << "\n";
        }
        nodesIt.next();
    }
}

bool _Mapping::mapActorToCurrentMayaCharacter(QString actorID)
{
    // get active maya character name
    QString activeCharacterName = Mapping::get()->getCurrentMayaCharacter();
    if(activeCharacterName.isEmpty()) {
        Utils::spawnMayaError("Active character is empty!");
        return false;
    }
    // find hips joint by character name and NAME_Hips pattern
    MSelectionList hipsLs;
    QString hipsBoneName = QString("%1_Hips").arg(activeCharacterName);
    MStatus hipsFound = MGlobal::getSelectionListByName(MString(hipsBoneName.toStdString().c_str()), hipsLs);
    if(hipsFound != MStatus::kSuccess) {
        Utils::spawnMayaError(QString("Unable to map character! %1").arg(activeCharacterName));
        return false;
    }
    MObject hipsObject;
    MDagPath hipsDagPath;
    hipsLs.getDependNode(0, hipsObject);
    hipsLs.getDagPath(0, hipsDagPath);
    // map rs character id to this character hips object adding RokokoMapping custom attribute to it
    objectsMap.insert(actorID, hipsObject);
    // append custom attribute to hips joint
    Mapping::get()->setOrCreateRSIdAttribute(hipsDagPath.fullPathName().asChar(), actorID);
    return true;
}

void _Mapping::unmapMayaObjectByName(QString mayaObjecName)
{
    QFile cmdFile(":/resources/deleteMappingAttribute.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME);
    cmdString.replace("MAYA_OBJECT_NAME", mayaObjecName);
    MGlobal::executeCommand(cmdString.toStdString().c_str());

    syncMapping();

}

void _Mapping::setOrCreateRSIdAttribute(QString mayaObjecName, QString value)
{
    QFile cmdFile(":/resources/setOrCreateRSIdAttribute.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("RS_ID_TAG", value);
    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME);
    cmdString.replace("MAYA_OBJECT_NAME", mayaObjecName);
    MGlobal::executeCommand(cmdString.toStdString().c_str());
}

void _Mapping::createHIKForActor(QString rsObjectID)
{
    QFile cmdFile(":/resources/createHIKForActor.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("ACTOR_ID", rsObjectID);
    MString actorCreated = MGlobal::executeCommandStringResult(cmdString.toStdString().c_str());
    if(actorCreated == "1") {
        QTimer::singleShot(500, []() {
            MGlobal::executeCommandOnIdle("hikToggleLockDefinition();");
        });
    } else {
        Utils::mayaPrintMessage("Character with this name already created!");
    }
}

QString _Mapping::getCurrentMayaCharacter()
{
    MString currentCharacterName = MGlobal::executeCommandStringResult("hikGetCurrentCharacter();");
    return QString(currentCharacterName.asChar());
}

void _Mapping::clear()
{
    objectsMap.clear();
//    boneMapping.clear();
}

void _Mapping::resetCallbacks()
{
    for(MCallbackId id : callbacks) {
        MSceneMessage::removeCallback(id);
    }
}

const QMultiMap<QString, MObject> &_Mapping::getObjectMapping()
{
    return objectsMap;
}

const QHash<QString, QString> &_Mapping::getBoneMapping()
{
    return boneMapping;
}

const QHash<QString, MQuaternion> &_Mapping::getStudioTPose()
{
    return studioTPose;
}
