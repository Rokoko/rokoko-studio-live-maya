#ifndef _MAPPING_H
#define _MAPPING_H

#include "singleton.h"
#include <maya/MObject.h>
#include <maya/MQuaternion.h>
#include <maya/MCallbackIdArray.h>
#include <QMultiMap>
#include <QHash>



class _Mapping
{
public:
    _Mapping();
    void mapRSObjectToSelection(QString rsObjectID);
    // unmap everything from this object
    void unmapRSObject(QString rsObjectID, bool selected);
    // select mapped objects
    void selectObjects(QString rsObjectID);
    // we need to populate mapped objects when plugin
    // starts or rs objects scene changed
    void syncMapping();

    // map actor id to maya hik root bone (hips)
    bool mapActorToCurrentMayaCharacter(QString actorID);
    void unmapMayaObjectByName(QString mayaObjecName);

    void setOrCreateRSIdAttribute(QString mayaObjecName, QString rsObjectID);

    void createHIKForActor(QString rsObjectID);
    QString getCurrentMayaCharacter();

    void clear();
    void resetCallbacks();

    const QMultiMap<QString, MObject> &getObjectMapping();
    const QHash<QString, QString> &getBoneMapping();
    const QHash<QString, MQuaternion> &getStudioTPose();
private:
    // prop id - maya object
    QMultiMap<QString, MObject> objectsMap;
    // maya hik bone name to rs bone name
    QHash<QString, QString> boneMapping;
    // studio t-pose
    QHash<QString, MQuaternion> studioTPose;

    MCallbackIdArray callbacks;
};


typedef Singleton<_Mapping> Mapping;

#endif // _MAPPING_H
