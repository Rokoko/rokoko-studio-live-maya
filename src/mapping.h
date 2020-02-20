#ifndef _MAPPING_H
#define _MAPPING_H

#include "singleton.h"
#include <QHash>


class _Mapping
{
public:
    _Mapping();
    void setProps(const QHash<QString, QString>&);
private:
    // prop id - maya uuid
    QHash<QString, QString> propsMap;
};


typedef Singleton<_Mapping> Mapping;

#endif // _MAPPING_H
