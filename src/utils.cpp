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
    if(MGlobal::isYAxisUp())
        return MVector(-rsTranslation.x, rsTranslation.y, rsTranslation.z);
    else
        return MVector(-rsTranslation.x, -rsTranslation.z, rsTranslation.y);
}

MQuaternion Utils::rsToMaya(MQuaternion rsRotation)
{
    if(MGlobal::isYAxisUp())
        return MQuaternion(-rsRotation.x, rsRotation.y, rsRotation.z, -rsRotation.w);
    else
        return MQuaternion(rsRotation.x, rsRotation.z, -rsRotation.y, rsRotation.w);
}

