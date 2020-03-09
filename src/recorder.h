#ifndef RECORDER_H
#define RECORDER_H

#include "singleton.h"

#include <QString>
#include <QHash>
#include <QPair>

#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MDagPath.h>
#include <maya/MString.h>


class FrameData {

public:
    // objects
    // object name - TR
    QHash<QString, QPair<MVector, MQuaternion>> objects;
    //faces
    // node path - weight name - weight value
    QHash<QString, QHash<QString, float>> face;
    // characters
    //...
};


class _Recorder
{
public:
    _Recorder();

    void recordPropOrTracker(float timestamp, MDagPath path, MVector location, MQuaternion rotation);
    void finalizeRecording();

    static int getFrame(const QList<float> &timestamps, int frameNumber);

    static int getCorrectedFrameNumber(const QList<float> &timestamps, int frameIndex);

    void keyframeDoubleAttribute(MString attrName, int frame, MDagPath dagPath, double value);

    QList<float> sortedTimeStamps();
private:
    QHash<float, FrameData> recordedData;
};


typedef Singleton<_Recorder> Recorder;

#endif // RECORDER_H
