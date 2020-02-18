#include "ui/rootwidget.h"
#include "constants.h"

#include <QPointer>
#include <QVBoxLayout>

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MQtUtil.h>
#include <maya/MTimerMessage.h>

QPointer<RootWidget> rsmlWidget;
QPointer<QWidget> workspaceControl;


MStatus initializePlugin(MObject) {
    MStatus result = MStatus::kSuccess;

    MString cmd;
    char sizeArgs[128];
    sprintf(sizeArgs, "-iw %d -mw %d -ih %d -mh %d", INITIAL_WINDOW_WIDTH, MINIMUM_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, MINIMUM_WINDOW_HEIGHT);
    cmd.format("workspaceControl -l \"^1s\" -fl true ^2s \"RSMLControl\"", WORKEPACE_CONTROL_NAME, sizeArgs);
    MGlobal::executeCommand(cmd);
    workspaceControl = MQtUtil::findControl("RSMLControl");
    rsmlWidget = new RootWidget(workspaceControl);
    return result;
}


MStatus uninitializePlugin(MObject plugin)
{
    MStatus result = MStatus::kSuccess;

    if (!workspaceControl.isNull()) {
        MGlobal::executeCommand("workspaceControl -e -close \"RSMLControl\"");
        MGlobal::executeCommand("deleteUI -wnd \"RSMLControl\"");
        workspaceControl.clear();
    }

    if(!rsmlWidget.isNull()) {
        rsmlWidget.clear();
    }

    return result;
}
