#ifndef _MAPPING_H
#define _MAPPING_H

#include "singleton.h"
#include <maya/MObject.h>
#include <QMultiMap>



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

    void createHIKForActor(QString rsObjectID);

    const QMultiMap<QString, MObject>& getMapping() { return objectsMap; }
private:
    // prop id - maya object
    QMultiMap<QString, MObject> objectsMap;
};


typedef Singleton<_Mapping> Mapping;

#endif // _MAPPING_H
