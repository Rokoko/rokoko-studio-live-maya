#include "recorder.h"

_Recorder::_Recorder()
{

}

void _Recorder::recordPropOrTracker(float timestamp, MDagPath path, MVector location, MQuaternion rotation)
{
    std::cout << timestamp << "\n";

}

void _Recorder::finalizeRecording()
{
    std::cout << "finalize recording!\n";
}
