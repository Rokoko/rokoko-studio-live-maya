#include "utils.h"

#include "mapping.h"
#include "animations.h"
#include "recorder.h"
#include "constants.h"

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

void Utils::removeMayaWSControl()
{
    QString closeCmd = QString("if (`window -ex \"%1\"`); workspaceControl -e -close \"%1\";").arg(WORKSPACE_CONTROL_NAME);
    QString deleteCmd = QString("if (`window -ex \"%1\"`); deleteUI -wnd \"%1\";").arg(WORKSPACE_CONTROL_NAME);

    MGlobal::executeCommand(closeCmd.toStdString().c_str());
    MGlobal::executeCommand(deleteCmd.toStdString().c_str());
}

void Utils::RSLMInit()
{
    Mapping::get()->installCallbacks();
    Mapping::get()->syncMapping();
}

void Utils::RSLMShutdown()
{
    Recorder::get()->reset();
    Mapping::get()->reset();
    Animations::get()->reset();
}
