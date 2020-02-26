#include <iostream>
#include "mapping.h"
#include "utils.h"
#include <QFile>
#include <QTimer>
#include <maya/MFn.h>
#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItSelectionList.h>


const QString MAPPING_FILED_NAME = "RokokoMapping";


_Mapping::_Mapping()
{

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
    if(activeCharacterName.isEmpty()) return false;
    // find hips joint by character name and NAME_Hips pattern
    MSelectionList hipsLs;
    QString hipsBoneName = QString("%1_Hips").arg(activeCharacterName);
    MStatus hipsFound = MGlobal::getSelectionListByName(MString(hipsBoneName.toStdString().c_str()), hipsLs);
    if(hipsFound != MStatus::kSuccess) return false;
    MObject hipsObject;
    hipsLs.getDependNode(0, hipsObject);
    // map rs character id to this character hips object adding RokokoMapping custom attribute to it
    objectsMap.insert(actorID, hipsObject);
    return true;
}

void _Mapping::createHIKForActor(QString rsObjectID)
{
    QFile cmdFile(":/resources/createHIKForActor.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdFile.close();

    cmdString.replace("ACTOR_ID", rsObjectID);
    MGlobal::executeCommand(cmdString.toStdString().c_str());
    QTimer::singleShot(500, []() {
        MGlobal::executeCommand("hikToggleLockDefinition();");
    });
}

QString _Mapping::getCurrentMayaCharacter()
{
    MString currentCharacterName = MGlobal::executeCommandStringResult("hikGetCurrentCharacter();");
    std::cout << currentCharacterName << " < Char name\n";
    return QString(currentCharacterName.asChar());
}
