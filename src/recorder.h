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



class FrameData {

public:
    // objects
    // object name - TR
    QHash<QString, QPair<MVector, MQuaternion>> objects;

    //faces
    QList<std::function<void(int)>> setFaceKeyframDelegates;

    // characters
    //...
};


class _Recorder
{
public:
    _Recorder();

    void recordPropOrTracker(float timestamp, MDagPath path, MVector location, MQuaternion rotation);
    void recordFace(float timestamp, std::function<void(int)> foo);

    void finalizeRecording();

    static int getFrame(const QList<float> &timestamps, int frameNumber);

    static int getCorrectedFrameNumber(const QList<float> &timestamps, int frameIndex);

    void recordingToggled(bool enabled);

    void keyframeNumericAttribute(MString attrName, int frame, MDagPath dagPath, double value);
    void keyframeNumericAttribute(int frame, MPlug plug, double value);

    bool recordingEnabled() { return isRecording; }
    unsigned int numFramesAvailable() { return recordedData.count(); }
    float recordingStartTime() {return mRecordingStartTime ;}

    QList<float> sortedTimeStamps();
private:
    QHash<float, FrameData> recordedData;
    float mRecordingStartTime;
    bool isRecording = false;
};


typedef Singleton<_Recorder> Recorder;

#endif // RECORDER_H
