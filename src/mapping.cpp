#include "mapping.h"

_Mapping::_Mapping()
{

}

void _Mapping::setProps(const QHash<QString, QString>& newProps)
{
    propsMap = newProps;
}
