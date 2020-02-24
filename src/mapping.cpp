#include <iostream>
#include "mapping.h"
#include "utils.h"
#include <QFile>
#include <maya/MGlobal.h>


const QString MAPPING_FILED_NAME = "RokokoMapping";


_Mapping::_Mapping()
{

}

void _Mapping::mapRSObjectToSelection(QString rsObjectID)
{
    QFile cmdFile(":/resources/mapToSelectedObjects.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME);
    cmdString.replace("RS_ID_TAG", rsObjectID);
    MGlobal::executeCommand(cmdString.toStdString().c_str());
    cmdFile.close();
}

void _Mapping::unmapRSObject(QString rsObjectID)
{
    QFile cmdFile(":/resources/unmapRSObject.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME);
    cmdString.replace("RS_ID_TAG", rsObjectID);
    MGlobal::executeCommand(cmdString.toStdString().c_str());
    cmdFile.close();
}

void _Mapping::selectObjects(QString rsObjectID)
{
    QFile cmdFile(":/resources/selectMappedObjects.mel");
    cmdFile.open(QFile::ReadOnly);
    QString cmdString = cmdFile.readAll();
    cmdString.replace("MAPPING_FIELD_NAME", MAPPING_FILED_NAME);
    cmdString.replace("RS_ID_TAG", rsObjectID);
    MGlobal::executeCommand(cmdString.toStdString().c_str());
    cmdFile.close();
}

void _Mapping::setProps(const QHash<QString, QString>& newProps)
{
    propsMap = newProps;
}
