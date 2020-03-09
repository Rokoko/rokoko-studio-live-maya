#include "utils.h"
#include "constants.h"
#include "recorder.h"

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

void _Recorder::recordPropOrTracker(float timestamp, MDagPath path, MVector location, MQuaternion rotation)
{
    // create frame object if not exists
    if(!recordedData.contains(timestamp)) {
        recordedData[timestamp] = FrameData();
    }

    // put transform for objects
    if(!recordedData[timestamp].objects.contains(path.fullPathName().asChar()))
        recordedData[timestamp].objects.insert(path.fullPathName().asChar(), { location, rotation });

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

        FrameData fData = recordedData[ts];

        // bake props or trackers
        if(fData.objects.count() > 0)
        {
            for(QString path : fData.objects.keys())
            {
                MSelectionList ls;
                ls.add(path.toStdString().c_str(), false);
                MDagPath dagPath;
                ls.getDagPath(0, dagPath);

                auto kfData = fData.objects[path];
                keyframeDoubleAttribute("tx", frame, dagPath, kfData.first.x);
                keyframeDoubleAttribute("ty", frame, dagPath, kfData.first.y);
                keyframeDoubleAttribute("tz", frame, dagPath, kfData.first.z);

                MEulerRotation euRot = kfData.second.asEulerRotation();
                keyframeDoubleAttribute("rx", frame, dagPath, euRot.x);
                keyframeDoubleAttribute("ry", frame, dagPath, euRot.y);
                keyframeDoubleAttribute("rz", frame, dagPath, euRot.z);
            }
        }
    }

    recordedData.clear();
}

int _Recorder::getFrame(const QList<float> &timestamps, int frameNumber)
{
    return int(round((timestamps[frameNumber] - timestamps[0]) * RECEIVER_FPS));
}

int _Recorder::getCorrectedFrameNumber(const QList<float> &timestamps, int frameIndex)
{
    int currFrame = getFrame(timestamps, frameIndex);
    if (0 < frameIndex < timestamps.count() - 1)
    {
        int prev_frame = getFrame(timestamps, frameIndex - 1);
        int next_frame = getFrame(timestamps, frameIndex + 1);
        if (prev_frame == currFrame && next_frame == currFrame + 2)
            currFrame += 1;
        if (next_frame == currFrame && prev_frame == currFrame - 2)
            currFrame -= 1;
    }

    if (frameIndex == timestamps.count() - 1)
    {
        int prev_frame = getFrame(timestamps, frameIndex - 1);
        if (prev_frame == currFrame)
            currFrame += 1;
    }
    return currFrame;
}

void _Recorder::keyframeDoubleAttribute(MString attrName, int frame, MDagPath dagPath, double value)
{
    MFnAnimCurve curveFn;
    MFnDagNode fn(dagPath);
    MPlug txPlug = fn.findPlug(attrName, false, nullptr);
    MFnAnimCurve::AnimCurveType curveType = curveFn.timedAnimCurveTypeForPlug(txPlug);

    // get or create curve function set
    MObject txCurve;
    MPlugArray srcPlugs;
    if (txPlug.connectedTo(srcPlugs, true, false))
        curveFn.setObject(srcPlugs[0].node());
    else
        txCurve = curveFn.create(txPlug, curveType);

    curveFn.addKey(MTime(frame, MTime::uiUnit()), value);
}

QList<float> _Recorder::sortedTimeStamps()
{
    QList<float> timestamps = recordedData.keys();
    std::sort(timestamps.begin(), timestamps.end());
    return timestamps;
}
