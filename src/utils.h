#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QHash>
#include <maya/MObject.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>

class Utils {

public:
    static void spawnMayaError(QString message);
    static void mayaPrintMessage(QString message);
    static MVector rsToMaya(MVector);
    static MQuaternion rsToMaya(MQuaternion);
    static void fillFaceWeightsMap(const MFnBlendShapeDeformer &bsFn, QHash<QString, MPlug> &map);
    static void showWSControl();
    static void removeMayaWSControl();
    static void RSLMInit();
    static void RSLMShutdown();
    static void getObjectFromString(const MString &fullPath, MObject &object, MDagPath &path, MStatus &result);
};


#endif // UTILS_H
