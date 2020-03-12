#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "singleton.h"
#include "recorder.h"

#include <QHash>
#include <QJsonObject>

#include <maya/MDagPath.h>


class _Animations
{
public:
    _Animations();
    void putProps(const QHash<QString, QJsonObject>&);
    QHash<QString, QJsonObject> &getProps();
    void putTrackers(const QHash<QString, QJsonObject>&);
    QHash<QString, QJsonObject> &getTrackers();
    void putActors(const QHash<QString, QJsonObject>&);
    QHash<QString, QJsonObject> &getActors();
    void putFaces(const QHash<QString, QJsonObject>&);
    QHash<QString, QJsonObject> &getFaces();

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
    // prop id - json
    QHash<QString, QJsonObject> propsMap;
    // tracker name - json
    QHash<QString, QJsonObject> trackersMap;
    // actor id - json
    QHash<QString, QJsonObject> actorsMap;
    // faceId - json
    QHash<QString, QJsonObject> facesMap;

    void animatePropOrTracker(QJsonObject obj, const MDagPath &dagPath);

    bool recordingEnabled = false;
    std::function<void()> stopReceiverCallback = [](){};
};

typedef Singleton<_Animations> Animations;

#endif // ANIMATIONS_H

