#ifndef _MAPPING_H
#define _MAPPING_H

#include "singleton.h"
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
    void setProps(const QHash<QString, QString>&);
private:
    // prop id - maya uuid
    QHash<QString, QString> objectsMap;
};


typedef Singleton<_Mapping> Mapping;

#endif // _MAPPING_H
