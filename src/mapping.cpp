#include <iostream>
#include "mapping.h"
#include "utils.h"

#include <QFile>
#include <QTimer>
#include <QRegExp>

#include <maya/MFn.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItSelectionList.h>
#include <maya/MMessage.h>
#include <maya/MSceneMessage.h>
#include <maya/MAngle.h>
#include <maya/MPlug.h>


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

    boneMapping["LeftHandThumb1"] = "leftThumbProximal";
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
    boneMapping["LeftHandPinky4"] = "leftLittleTip";

    // right hand
    boneMapping["RightShoulder"] = "rightShoulder";
    boneMapping["RightArm"] = "rightUpperArm";
    boneMapping["RightForeArm"] = "rightLowerArm";
    boneMapping["RightHand"] = "rightHand";

    boneMapping["RightHandThumb1"] = "rightThumbProximal";
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
    boneMapping["RightHandPinky4"] = "rightLittleTip";

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

    studioTPose["leftThumbProximal"] = MQuaternion(-0.560, -0.701, 0.430, -0.090);
    studioTPose["leftThumbMedial"] = MQuaternion(-0.653, -0.653, 0.270, -0.270);
    studioTPose["leftThumbDistal"] = MQuaternion(-0.653, -0.653, 0.270, -0.2706);
    studioTPose["leftThumbTip"] = MQuaternion(-0.653, -0.653, 0.270, -0.270);
    studioTPose["leftIndexProximal"] = MQuaternion(-0.500, -0.500, 0.500, -0.500);
    studioTPose["leftMiddleProximal"] = MQuaternion(-0.500, -0.500, 0.500, -0.500);
    studioTPose["leftRingProximal"] = MQuaternion(-0.500, -0.500, 0.500, -0.500);
    studioTPose["leftLittleProximal"] = MQuaternion(-0.500, -0.500, 0.500, -0.500);

    studioTPose["rightThumbProximal"] = MQuaternion(0.560, -0.700, 0.500, 0.000);
    studioTPose["rightThumbMedial"] = MQuaternion(0.653, -0.653, 0.270, 0.270);
    studioTPose["rightThumbDistal"] = MQuaternion(0.653, -0.653, 0.270, 0.270);
    studioTPose["rightThumbTip"] = MQuaternion(0.653, -0.653, 0.270, 0.270);
    studioTPose["rightIndexProximal"] = MQuaternion(0.500, -0.500, 0.500, 0.500);
    studioTPose["rightMiddleProximal"] = MQuaternion(0.500, -0.500, 0.500, 0.500);
    studioTPose["rightRingProximal"] = MQuaternion(0.500, -0.500, 0.500, 0.500);
    studioTPose["rightLittleProximal"] = MQuaternion(0.500, -0.500, 0.500, 0.500);

    installCallbacks();

    faceShapeNames << "eyeBlinkLeft"
                   << "eyeLookDownLeft"
                   << "eyeLookInLeft"
                   << "eyeLookOutLeft"
                   << "eyeLookUpLeft"
                   << "eyeSquintLeft"
                   << "eyeWideLeft"
                   << "eyeBlinkRight"
                   << "eyeLookDownRight"
                   << "eyeLookInRight"
                   << "eyeLookOutRight"
                   << "eyeLookUpRight"
                   << "eyeSquintRight"
                   << "eyeWideRight"
                   << "jawForward"
                   << "jawLeft"
                   << "jawRight"
                   << "jawOpen"
                   << "mouthClose"
                   << "mouthFunnel"
                   << "mouthPucker"
                   << "mouthLeft"
                   << "mouthRight"
                   << "mouthSmileLeft"
                   << "mouthSmileRight"
                   << "mouthFrownLeft"
                   << "mouthFrownRight"
                   << "mouthDimpleLeft"
                   << "mouthDimpleRight"
                   << "mouthStretchLeft"
                   << "mouthStretchRight"
                   << "mouthRollLower"
                   << "mouthRollUpper"
                   << "mouthShrugLower"
                   << "mouthShrugUpper"
                   << "mouthPressLeft"
                   << "mouthPressRight"
                   << "mouthLowerDownLeft"
                   << "mouthLowerDownRight"
                   << "mouthUpperUpLeft"
                   << "mouthUpperUpRight"
                   << "browDownLeft"
                   << "browDownRight"
                   << "browInnerUp"
                   << "browOuterUpLeft"
                   << "browOuterUpRight"
                   << "cheekPuff"
                   << "cheekSquintLeft"
                   << "cheekSquintRight"
                   << "noseSneerLeft"
                   << "noseSneerRight"
                   << "tongueOut";

}

void _Mapping::mapRSObjectToSelection(QString rsObjectID)
{
    QFile cmdFile(":/resources/mapToSelectedObjects.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME.asChar());
    cmdString.replace("RS_ID_TAG", rsObjectID);
    MGlobal::executeCommand(cmdString.toStdString().c_str());

    syncMapping();
}

void _Mapping::unmapRSObject(QString rsObjectID, bool selected=false)
{
    QFile cmdFile(":/resources/unmapRSObject.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME.asChar());
    cmdString.replace("RS_ID_TAG", rsObjectID);
    cmdString.replace("SELECTED_ONLY", selected ? "true" : "false");
    MGlobal::executeCommand(cmdString.toStdString().c_str());

    syncMapping();
}

void _Mapping::selectObjects(QString rsObjectID)
{
    QFile cmdFile(":/resources/selectMappedObjects.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME.asChar());
    cmdString.replace("RS_ID_TAG", rsObjectID);
    MGlobal::executeCommand(cmdString.toStdString().c_str());
}

void _Mapping::syncMapping()
{
    // erase all
    objectsMap.clear();
    faceObjectsMap.clear();

    // sync transform nodes
    MItDependencyNodes nodesIt(MFn::kTransform);
    while(!nodesIt.isDone()) {
        MObject object = nodesIt.thisNode();
        MFnDependencyNode fn(object);
        MString fieldName(MAPPING_FILED_NAME.asChar());
        bool attrFound = fn.hasAttribute(fieldName);
        if(attrFound)
        {
            MStatus plugFound;
            MString rsIdValue = fn.findPlug(fieldName, plugFound).asString();

            MDagPath path;
            MDagPath::getAPathTo(object, path);
            objectsMap.insert(rsIdValue.asChar(), path.fullPathName());
        }
        nodesIt.next();
    }

    // sync blend shapes
    MItDependencyNodes bsIt(MFn::kBlendShape);
    while(!bsIt.isDone()) {
        MObject object = bsIt.thisNode();
        MFnDependencyNode fn(object);
        MString faceMappingAttributeName(FACE_MAPPING_FILED_NAME.asChar());
        bool mappingFound = fn.hasAttribute(faceMappingAttributeName);
        if(mappingFound) {
            // get rs id
            MStatus plugFound;
            MPlug faceMappingPlug = fn.findPlug(FACE_MAPPING_FILED_NAME.asChar(), true, &plugFound);
            if (!faceMappingPlug.isNull()) {
                if(faceMappingPlug.isCompound()) {
                    unsigned int numChildren = faceMappingPlug.numChildren();
                    MPlug faceIdPlug;
                    for (unsigned int i = 0; i < numChildren; ++i) {
                        MPlug childPlug = faceMappingPlug.child(i);
                        if(childPlug.partialName() == PREFIXED_FACE_ID) {
                            faceIdPlug = childPlug;
                            break;
                        }
                    }
                    if(!faceIdPlug.isNull())
                    {
                        MString faceId = faceIdPlug.asString();
                        faceObjectsMap.insert(faceId.asChar(), object);
                    }
                }
            }
        }

        bsIt.next();
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
    // TODO: check if namespace break this
    MSelectionList hipsLs;
    QString charHipsCmd = QString("hikGetSkNode(\"%1\", 1);").arg(activeCharacterName);
    MString hipsBoneName = MGlobal::executeCommandStringResult(charHipsCmd.toStdString().c_str());
    MStatus hipsFound = MGlobal::getSelectionListByName(hipsBoneName, hipsLs);
    if(hipsFound != MStatus::kSuccess) {
        Utils::spawnMayaError(QString("Unable to map character! %1").arg(activeCharacterName));
        return false;
    }
    MObject hipsObject;
    MDagPath hipsDagPath;
    hipsLs.getDependNode(0, hipsObject);
    hipsLs.getDagPath(0, hipsDagPath);
    // map rs character id to this character hips object adding RokokoMapping custom attribute to it
    objectsMap.insert(actorID, hipsDagPath.fullPathName());
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

    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME.asChar());
    cmdString.replace("MAYA_OBJECT_NAME", mayaObjecName);
    MGlobal::executeCommand(cmdString.toStdString().c_str());

    syncMapping();

}

void _Mapping::setOrCreateRSIdAttribute(QString mayaObjecName, QString rsId)
{
    QFile cmdFile(":/resources/setOrCreateRSIdAttribute.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("RS_ID_TAG", rsId);
    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME.asChar());
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

void _Mapping::mapFaceToMayaObject(QString mayaObjecName, QString rsId)
{
    // first grab object ref
    MSelectionList ls;
    MGlobal::getSelectionListByName(mayaObjecName.toStdString().c_str(), ls);

    if(ls.length() == 0)
        return;

    MObject object;
    ls.getDependNode(0, object);
    MFnDependencyNode fn(object);

    // fetch selected node connected blendshapes
    QFile cmdFile(":/resources/fetchBlendShapes.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();
    cmdString.replace("MAYA_OBJECT_NAME", fn.name().asChar());

    cmdString.replace("SEPARATOR", BS_SEPARATOR);
    MString connectedBlendShapes = MGlobal::executeCommandStringResult(cmdString.toStdString().c_str());
    if(connectedBlendShapes.length() == 0) {
        Utils::spawnMayaError("Object have no blend shapes connected!");
        return;
    }
    QStringList blendShapesList = QString(connectedBlendShapes.asChar()).split(BS_SEPARATOR);

    // for each blendshape
    // creafe face attributes

    MFnTypedAttribute tAttr;
    MFnCompoundAttribute compound;
    compound.setKeyable(false);
    compound.setStorable(true);
    compound.setWritable(true);
    compound.setReadable(true);

    struct Local {
        static void addAttr(MFnCompoundAttribute& parent, MFnTypedAttribute &tAttr, const MString& fieldName) {
            MString prefixedName = BLEND_SHAPE_PREFIX + fieldName;
            MObject obj = tAttr.create(prefixedName, prefixedName, MFnData::kString);
            parent.addChild(obj);
        }

        static void setAttributeString(MFnDependencyNode& node, const MString& fieldName, const MString& value) {
            MString prefixedName = BLEND_SHAPE_PREFIX + fieldName;
            MPlug plug = node.findPlug(prefixedName, true);
            plug.setString(value);
        }
    };

    for(QString bsNodeName : blendShapesList) {
        MSelectionList bsls;
        MGlobal::getSelectionListByName(bsNodeName.toStdString().c_str(), bsls);

        if(bsls.length() == 0)
            return;

        MObject object;
        bsls.getDependNode(0, object);
        MFnDependencyNode fn(object);

        if(fn.hasAttribute(FACE_MAPPING_FILED_NAME)) {
            Utils::mayaPrintMessage(QString("%1 of %2 node already mapped!").arg(bsNodeName, mayaObjecName));
            continue;
        }

        // create compound attribute
        MObject compoundObj = compound.create("RokokoFaceMapping", "RokokoFaceMapping");

        // create face id attribute
        Local::addAttr(compound, tAttr, "FaceId");
        Local::addAttr(compound, tAttr, "eyeBlinkLeft");
        Local::addAttr(compound, tAttr, "eyeLookDownLeft");
        Local::addAttr(compound, tAttr, "eyeLookInLeft");
        Local::addAttr(compound, tAttr, "eyeLookOutLeft");
        Local::addAttr(compound, tAttr, "eyeLookUpLeft");
        Local::addAttr(compound, tAttr, "eyeSquintLeft");
        Local::addAttr(compound, tAttr, "eyeWideLeft");
        Local::addAttr(compound, tAttr, "eyeBlinkRight");
        Local::addAttr(compound, tAttr, "eyeLookDownRight");
        Local::addAttr(compound, tAttr, "eyeLookInRight");
        Local::addAttr(compound, tAttr, "eyeLookOutRight");
        Local::addAttr(compound, tAttr, "eyeLookUpRight");
        Local::addAttr(compound, tAttr, "eyeSquintRight");
        Local::addAttr(compound, tAttr, "eyeWideRight");
        Local::addAttr(compound, tAttr, "jawForward");
        Local::addAttr(compound, tAttr, "jawLeft");
        Local::addAttr(compound, tAttr, "jawRight");
        Local::addAttr(compound, tAttr, "jawOpen");
        Local::addAttr(compound, tAttr, "mouthClose");
        Local::addAttr(compound, tAttr, "mouthFunnel");
        Local::addAttr(compound, tAttr, "mouthPucker");
        Local::addAttr(compound, tAttr, "mouthLeft");
        Local::addAttr(compound, tAttr, "mouthRight");
        Local::addAttr(compound, tAttr, "mouthSmileLeft");
        Local::addAttr(compound, tAttr, "mouthSmileRight");
        Local::addAttr(compound, tAttr, "mouthFrownLeft");
        Local::addAttr(compound, tAttr, "mouthFrownRight");
        Local::addAttr(compound, tAttr, "mouthDimpleLeft");
        Local::addAttr(compound, tAttr, "mouthDimpleRight");
        Local::addAttr(compound, tAttr, "mouthStretchLeft");
        Local::addAttr(compound, tAttr, "mouthStretchRight");
        Local::addAttr(compound, tAttr, "mouthRollLower");
        Local::addAttr(compound, tAttr, "mouthRollUpper");
        Local::addAttr(compound, tAttr, "mouthShrugLower");
        Local::addAttr(compound, tAttr, "mouthShrugUpper");
        Local::addAttr(compound, tAttr, "mouthPressLeft");
        Local::addAttr(compound, tAttr, "mouthPressRight");
        Local::addAttr(compound, tAttr, "mouthLowerDownLeft");
        Local::addAttr(compound, tAttr, "mouthLowerDownRight");
        Local::addAttr(compound, tAttr, "mouthUpperUpLeft");
        Local::addAttr(compound, tAttr, "mouthUpperUpRight");
        Local::addAttr(compound, tAttr, "browDownLeft");
        Local::addAttr(compound, tAttr, "browDownRight");
        Local::addAttr(compound, tAttr, "browInnerUp");
        Local::addAttr(compound, tAttr, "browOuterUpLeft");
        Local::addAttr(compound, tAttr, "browOuterUpRight");
        Local::addAttr(compound, tAttr, "cheekPuff");
        Local::addAttr(compound, tAttr, "cheekSquintLeft");
        Local::addAttr(compound, tAttr, "cheekSquintRight");
        Local::addAttr(compound, tAttr, "noseSneerLeft");
        Local::addAttr(compound, tAttr, "noseSneerRight");
        Local::addAttr(compound, tAttr, "tongueOut");

        // create shapes string attributes
        fn.addAttribute(compoundObj);

        Local::setAttributeString(fn, "FaceId", rsId.toStdString().c_str());

    }

    syncMapping();
}

void _Mapping::unmapFaceFromMayaObject(QString mayaObjecName)
{
    // grab object ref
    MSelectionList ls;
    MGlobal::getSelectionListByName(mayaObjecName.toStdString().c_str(), ls);

    if(ls.length() == 0)
        return;

    MObject object;
    ls.getDependNode(0, object);
    MFnDependencyNode fn(object);

    // fetch connected blendshapes
    QFile cmdFile(":/resources/fetchBlendShapes.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();
    cmdString.replace("MAYA_OBJECT_NAME", fn.name().asChar());

    cmdString.replace("SEPARATOR", BS_SEPARATOR);
    MString connectedBlendShapes = MGlobal::executeCommandStringResult(cmdString.toStdString().c_str());
    QStringList blendShapesList = QString(connectedBlendShapes.asChar()).split(BS_SEPARATOR);

    // iterate over blend shapes and remove face mapping compound attribute
    for(QString bsNodeName : blendShapesList)
    {
        MSelectionList bsls;
        MGlobal::getSelectionListByName(bsNodeName.toStdString().c_str(), bsls);

        if(bsls.length() == 0)
            return;

        MObject bsObject;
        bsls.getDependNode(0, bsObject);
        MFnDependencyNode fn(bsObject);
        if (fn.hasAttribute(FACE_MAPPING_FILED_NAME))
        {
            MPlug plug = fn.findPlug(FACE_MAPPING_FILED_NAME, true);
            fn.removeAttribute(plug.attribute());
        }
    }

    // sync mapping
    syncMapping();
}

void _Mapping::unmapAllFaces(QString rsId)
{
    // iterate over all blend shape nodes
    MItDependencyNodes bsIterator(MFn::kBlendShape);

    while (!bsIterator.isDone()) {
        MFnDependencyNode node(bsIterator.thisNode());
        if (node.hasAttribute(FACE_MAPPING_FILED_NAME.asChar())) {
            MPlug plug = node.findPlug(FACE_MAPPING_FILED_NAME.asChar(), true);

            MPlug faceIdPlug = node.findPlug(PREFIXED_FACE_ID, true);
            MString faceIdValue;
            faceIdPlug.getValue(faceIdValue);

            // remove face mapping with passed id
            if(faceIdValue.asChar() == rsId) {
                node.removeAttribute(plug.attribute());
            }
        }
        bsIterator.next();
    }
}

void _Mapping::automapWeights(QString rsId)
{

    // printf("Automap face\n");
    auto compareFaceWeightNames = [](QString studioName, QString foreignName) -> bool {
        QRegExp exp("[^A-Za-z]");
        QString strippedForeignName = foreignName.remove(exp);
        return strippedForeignName.toLower().contains(studioName.toLower());
    };

    // iterate over all blend shape nodes with this item id
    // try to put existing weight names into correct RS shape name field
    MItDependencyNodes bsIt(MFn::kBlendShape);
    while(!bsIt.isDone()) {
        MObject object = bsIt.thisNode();
        MFnBlendShapeDeformer fn(object);
        MString faceMappingAttributeName(FACE_MAPPING_FILED_NAME.asChar());
        bool mappingFound = fn.hasAttribute(faceMappingAttributeName);
        if(mappingFound) {
            // get rs id
            MStatus plugFound;
            MPlug faceMappingPlug = fn.findPlug(FACE_MAPPING_FILED_NAME.asChar(), true, &plugFound);
            if (!faceMappingPlug.isNull()) {
                if(faceMappingPlug.isCompound()) {
                    unsigned int numChildren = faceMappingPlug.numChildren();
                    MPlug faceIdPlug;
                    for (unsigned int i = 0; i < numChildren; ++i) {
                        MPlug childPlug = faceMappingPlug.child(i);
                        if(childPlug.partialName() == PREFIXED_FACE_ID) {
                            faceIdPlug = childPlug;
                            break;
                        }
                    }

                    MString faceId = faceIdPlug.asString();
                    if(faceId.asChar() == rsId) {
                        for(const QString& studioName : faceShapeNames)
                        {
                            QHash<QString, MPlug> weights;
                            Utils::fillFaceWeightsMap(fn, weights);
                            const QStringList weightNames = weights.keys();
                            for(const QString &foreignName : weightNames) {
                                if(compareFaceWeightNames(studioName, foreignName)) {
                                    // put
                                    QString prefixedStudioName = BLEND_SHAPE_PREFIX.asChar() + studioName;
                                    MPlug mappingShapePlug = fn.findPlug(prefixedStudioName.toStdString().c_str(), false);
                                    MStatus success = mappingShapePlug.setValue(foreignName.toStdString().c_str());
                                    if(success == MStatus::kSuccess) {
                                        //printf("Mapped\n");
                                    } else {
                                        //printf("Not mapped\n");
                                    }
                                }
                            }
                        }
                    } else {
                        //printf("Invalid face id %s - %s\n", faceId.asChar(), rsId.toStdString().c_str());
                        //printf("%d\n", QString(faceId.asChar()) == rsId);
                    }
                }
            } else {
                //printf("face mapping not found\n");
            }
        } else {
            //printf("Mapping not found\n");
        }

        bsIt.next();
    }
}

void _Mapping::clear()
{
    objectsMap.clear();
    faceObjectsMap.clear();
}

void _Mapping::reset()
{
    clear();
    resetCallbacks();
}

void _Mapping::installCallbacks()
{
    // register callbacks
    MCallbackId beforeNewId = MSceneMessage::addCheckCallback(MSceneMessage::kBeforeNewCheck, [](bool* recCode, void* clientData) {
        Q_UNUSED(clientData)
        Mapping::get()->clear();
        *recCode = true;
    });
    MCallbackId beforeOpenId = MSceneMessage::addCheckCallback(MSceneMessage::kBeforeOpenCheck, [](bool* recCode, void* clientData) {
        Q_UNUSED(clientData)
        Mapping::get()->clear();
        *recCode = true;
    });
    callbacks.append(beforeNewId);
    callbacks.append(beforeOpenId);
}

void _Mapping::resetCallbacks()
{
    for(MCallbackId id : callbacks) {
        MSceneMessage::removeCallback(id);
    }
    callbacks.clear();
}

const QMultiMap<QString, MString> &_Mapping::getObjectMapping()
{
    return objectsMap;
}

const QMultiMap<QString, MObject> &_Mapping::getFaceObjectMapping()
{
    return faceObjectsMap;
}

const QHash<QString, QString> &_Mapping::getBoneMapping()
{
    return boneMapping;
}

const QHash<QString, MQuaternion> &_Mapping::getStudioTPose()
{
    return studioTPose;
}
