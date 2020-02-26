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

MVector Utils::rsToMaya(MVector rsTranslation)
{
    return MVector(-rsTranslation.x, rsTranslation.y, rsTranslation.z);
}

MQuaternion Utils::rsToMaya(MQuaternion rsRotation)
{
    return MQuaternion(-rsRotation.x, rsRotation.y, rsRotation.z, -rsRotation.w);
}

