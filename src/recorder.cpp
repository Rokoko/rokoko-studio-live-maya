#include "utils.h"
#include "constants.h"
#include "recorder.h"

#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MAnimControl.h>
#include <maya/MObjectArray.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MAnimControl.h>
#include <maya/MTimeArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MAnimUtil.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MTime.h>


_Recorder::_Recorder()
{

}

void _Recorder::recordPropOrTracker(float timestamp, std::function<void(int)> foo)
{
    recordedData[timestamp].append(foo);
}

void _Recorder::recordFace(float timestamp, std::function<void(int)> foo)
{
    recordedData[timestamp].append(foo);
}

void _Recorder::recordBone(float timestamp, std::function<void (int)> foo)
{
    recordedData[timestamp].append(foo);
}

void _Recorder::finalizeRecording()
{
    QList<float> timestamps = sortedTimeStamps();

    MTime currentTime = MAnimControl::currentTime();

    int index = -1 + currentTime.value();
    for(float ts: timestamps)
    {
        index++;
        int frame = index;

        auto delegates = recordedData[ts];

        // bake
        for (auto foo : delegates) foo(frame);

    }

    recordedData.clear();
}

void _Recorder::recordingToggled(bool enabled)
{
    mRecordingStartTime = MAnimControl::currentTime().value();
    isRecording = enabled;
}

void _Recorder::keyframeNumericAttribute(MString attrName, int frame, MDagPath dagPath, double value)
{
    MFnAnimCurve curveFn;
    MFnDagNode fn(dagPath);
    MPlug plug = fn.findPlug(attrName, false, nullptr);
    keyframeNumericAttribute(frame, plug, value);
}

void _Recorder::keyframeNumericAttribute(int frame, MPlug plug, double value)
{
    MFnAnimCurve curveFn;
    MFnAnimCurve::AnimCurveType curveType = curveFn.timedAnimCurveTypeForPlug(plug);

    // get or create curve function set
    MObject curve;
    MPlugArray srcPlugs;
    if (plug.connectedTo(srcPlugs, true, false))
        curveFn.setObject(srcPlugs[0].node());
    else
        curve = curveFn.create(plug, curveType);

    curveFn.addKey(MTime(frame, MTime::uiUnit()), value);
}

QList<float> _Recorder::sortedTimeStamps()
{
    QList<float> timestamps = recordedData.keys();
    std::sort(timestamps.begin(), timestamps.end());
    return timestamps;
}

void _Recorder::reset()
{
    recordedData.clear();
}
