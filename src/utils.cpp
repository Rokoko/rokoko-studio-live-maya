#include "utils.h"
#include <QString>
#include <maya/MGlobal.h>
#include <maya/MString.h>


void Utils::spawnMayaError(QString message)
{
    QString cmd = QString("error -n \"%1\";").arg(message);
    MString mayaCmd(cmd.toStdString().c_str());
    MGlobal::executeCommand(mayaCmd);
}

void Utils::mayaPrintMessage(QString message)
{
    QString cmd = QString("print \"%1\";").arg(message);
    MString mayaCmd(cmd.toStdString().c_str());
    MGlobal::executeCommand(mayaCmd);
}

