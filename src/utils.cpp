#include "utils.h"

#include <QString>
#include <QHash>

#include <maya/MGlobal.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MString.h>
#include <maya/MPlug.h>


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

void Utils::fillFaceWeightsMap(const MFnBlendShapeDeformer &bsFn, QHash<QString, MPlug> &map)
{
    MStatus plugFound = MStatus::kFailure;
    MPlug weightsArray = bsFn.findPlug("weight", false, &plugFound);
    unsigned int weightsCount = weightsArray.numElements();
    for(unsigned int i=0; i < weightsCount; ++i)
    {
        MPlug shapePlug = weightsArray.elementByPhysicalIndex(i);
        MString shapeName = shapePlug.partialName(false, false, false, true, false, false);
        map.insert(shapeName.asChar(), shapePlug);
    }

    if(plugFound == MStatus::kFailure) {
        printf("Failed to find weight attribute!!");
    }
}
