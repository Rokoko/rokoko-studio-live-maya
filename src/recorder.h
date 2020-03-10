#ifndef RECORDER_H
#define RECORDER_H

#include <functional>

#include "singleton.h"

#include <QString>
#include <QHash>
#include <QPair>

#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MDagPath.h>
#include <maya/MString.h>
#include <maya/MFnBlendShapeDeformer.h>


class _Recorder
{
public:
    _Recorder();

    void recordPropOrTracker(float timestamp, std::function<void(int)> foo);
    void recordFace(float timestamp, std::function<void(int)> foo);
    void recordBone(float timestamp, std::function<void(int)> foo);

    void finalizeRecording();

    void recordingToggled(bool enabled);

    void keyframeNumericAttribute(MString attrName, int frame, MDagPath dagPath, double value);
    void keyframeNumericAttribute(int frame, MPlug plug, double value);

    bool recordingEnabled() { return isRecording; }
    unsigned int numFramesAvailable() { return recordedData.count(); }
    float recordingStartTime() {return mRecordingStartTime ;}

    QList<float> sortedTimeStamps();
    void reset();
private:
    // timestamp - set keyframe function
    QHash<float, QList<std::function<void(int)>>> recordedData;
    float mRecordingStartTime;
    bool isRecording = false;
};


typedef Singleton<_Recorder> Recorder;

#endif // RECORDER_H
