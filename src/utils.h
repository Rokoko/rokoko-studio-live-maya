#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <maya/MObject.h>

class Utils {

public:
    static void spawnMayaError(QString message);
    static void mayaPrintMessage(QString message);

};


#endif // UTILS_H
