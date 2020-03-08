#ifndef RECORDER_H
#define RECORDER_H

#include "singleton.h"

#include <QString>
#include <QHash>

#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MDagPath.h>


class _Recorder
{
public:
    _Recorder();

    void recordPropOrTracker(float timestamp, MDagPath path, MVector location, MQuaternion rotation);
    void finalizeRecording();
private:

};


typedef Singleton<_Recorder> Recorder;

#endif // RECORDER_H
