#include "ui/rootwidget.h"
#include "constants.h"
#include "utils.h"

#include <QPointer>
#include <QScrollArea>
#include <QString>

#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MQtUtil.h>
#include <maya/MTimerMessage.h>

#ifdef _WINDOWS
    #pragma comment(lib,"OpenMayaAnim.lib")
#endif

QPointer<QScrollArea> rsmlScrollArea;
QPointer<RootWidget> rsmlWidget;
QPointer<QWidget> workspaceControl;


class InvokeRSLM : MPxCommand {
public:
    InvokeRSLM() {}

    ~InvokeRSLM() {

    }

    MStatus doIt(const MArgList &args) override
    {
        Q_UNUSED(args);

        MStatus result = MStatus::kSuccess;

        if(!rsmlWidget.isNull()) {
            // do nothing if already visible
            if(rsmlWidget->isVisible()) return MStatus::kFailure;
        }


        if(workspaceControl.isNull())
        {
            MString cmd;
            char sizeArgs[128];
            sprintf(sizeArgs, "-iw %d -mw %d -ih %d -mh %d", INITIAL_WINDOW_WIDTH, MINIMUM_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, MINIMUM_WINDOW_HEIGHT);
            cmd.format("workspaceControl -l \"^1s\" -fl true ^2s \"RSMLControl\";", WIDGET_TITLE, sizeArgs);
            MGlobal::executeCommand(cmd);
        }

        workspaceControl = MQtUtil::findControl(WORKSPACE_CONTROL_NAME);
        rsmlScrollArea = new QScrollArea(workspaceControl);
        rsmlWidget = new RootWidget(workspaceControl);
        rsmlScrollArea->setWidget(rsmlWidget);
        rsmlScrollArea->setWidgetResizable(true);
        MQtUtil::addWidgetToMayaLayout(rsmlScrollArea, workspaceControl);

        return result;
    }

    static void* creator() {
        return new InvokeRSLM;
    }
};


MStatus initializePlugin(MObject obj) {
    MStatus result = MStatus::kSuccess;


    const char* versionString = QString("v%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH).toStdString().c_str();
    MFnPlugin plugin(obj, "Rokoko", versionString);
    result = plugin.registerCommand("showRSLM", InvokeRSLM::creator);

    return result;
}


MStatus uninitializePlugin(MObject obj)
{
    MStatus result = MStatus::kSuccess;

    MFnPlugin plugin(obj);

    plugin.deregisterCommand("showRSLM");

    if (!workspaceControl.isNull()) {
        // mel command will clear workspaceControl QPointer
        Utils::removeMayaWSControl();
    }

    if(!rsmlWidget.isNull()) {
        rsmlWidget.clear();
    }

    if(!rsmlScrollArea.isNull()) {
        rsmlScrollArea.clear();
    }

    Utils::RSLMShutdown();

    return result;
}
