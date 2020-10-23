#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "singleton.h"
#include "recorder.h"

#include <QHash>
#include <QJsonObject>

#include <maya/MDagPath.h>
#include "framework.h"



class _Animations
{
public:
    _Animations();
    void putProps(const QHash<QString, FPropSnapshot>&);
    QHash<QString, FPropSnapshot> &getProps();
    void putTrackers(const QHash<QString, FTrackerSnapshot>&);
    QHash<QString, FTrackerSnapshot> &getTrackers();
    void putActors(const QHash<QString, FActorSnapshot>&);
    QHash<QString, FActorSnapshot> &getActors();
    void putFaces(const QHash<QString, FFaceSnapshot>&);
    QHash<QString, FFaceSnapshot> &getFaces();

    void applyAnimationsToMappedObjects();

    void recordingToggled(bool enabled);

    void setStopReceiverCallback(std::function<void()> foo) { stopReceiverCallback = foo; }

    void setSceneScale(float);
    float sceneScale() { return _sceneScale; }
    float timestamp;
    float playbackTimestamp;
    void reset();
private:
    float _sceneScale = 1.0;
    // prop id - snapshot
    QHash<QString, FPropSnapshot> propsMap;
    // tracker name - snapshot
    QHash<QString, FTrackerSnapshot> trackersMap;
    // actor id - snapshot
    QHash<QString, FActorSnapshot> actorsMap;
    // faceId - snapshot
    QHash<QString, FFaceSnapshot> facesMap;

    void animatePropOrTracker(FPropSnapshot obj, const MDagPath &dagPath);
    void animatePropOrTracker(FTrackerSnapshot obj, const MDagPath &dagPath);

    bool recordingEnabled = false;
    std::function<void()> stopReceiverCallback = [](){};
};

typedef Singleton<_Animations> Animations;

#endif // ANIMATIONS_H

