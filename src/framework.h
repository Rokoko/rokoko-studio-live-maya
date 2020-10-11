#pragma once

#include <QString>
#include <QColor>
#include <QVector3D>
#include <QQuaternion>
#include <vector>
#include <map>


enum class EBodyJoints : uint8_t
{
    ROOT,
    HIP,
    STOMACH,
    CHEST,
    NECK,
    HEAD,
    LEFT_SHOULDER,
    LEFT_ARM,
    LEFT_FOREARM,
    LEFT_HAND,
    RIGHT_SHOULDER,
    RIGHT_ARM,
    RIGHT_FOREARM,
    RIGHT_HAND,
    LEFT_UP_LEG,
    LEFT_LEG,
    LEFT_FOOT,
    RIGHT_UP_LEG,
    RIGHT_LEG,
    RIGHT_FOOT,
    COUNT
};

enum class EFingerJoints : uint8_t
{
    PROXIMAL,
    MEDIAL,
    DISTAL,
    //TIP,
    COUNT
};

enum class EHandFingers : uint8_t
{
    THUMB,
    INDEX,
    MIDDLE,
    RING,
    LITTLE,
    COUNT
};

enum class EFaceShapes : uint8_t
{
    EYEBLINKLEFT,
    EYELOOKDOWNLEFT,
    EYELOOKINLEFT,
    EYELOOKOUTLEFT,
    EYELOOKUPLEFT,
    EYESQUINTLEFT,
    EYEWIDELEFT,
    EYEBLINKRIGHT,
    EYELOOKDOWNRIGHT,
    EYELOOKINRIGHT,
    EYELOOKOUTRIGHT,
    EYELOOKUPRIGHT,
    EYESQUINTRIGHT,
    EYEWIDERIGHT,
    JAWFORWARD,
    JAWLEFT,
    JAWRIGHT,
    JAWOPEN,
    MOUTHCLOSE,
    MOUTHFUNNEL,
    MOUTHPUCKER,
    MOUTHLEFT,
    MOUTHRIGHT,
    MOUTHSMILELEFT,
    MOUTHSMILERIGHT,
    MOUTHFROWNLEFT,
    MOUTHFROWNRIGHT,
    MOUTHDIMPLELEFT,
    MOUTHDIMPLERIGHT,
    MOUTHSTRETCHLEFT,
    MOUTHSTRETCHRIGHT,
    MOUTHROLLLOWER,
    MOUTHROLLUPPER,
    MOUTHSHRUGLOWER,
    MOUTHSHRUGUPPER,
    MOUTHPRESSLEFT,
    MOUTHPRESSRIGHT,
    MOUTHLOWERDOWNLEFT,
    MOUTHLOWERDOWNRIGHT,
    MOUTHUPPERUPLEFT,
    MOUTHUPPERUPRIGHT,
    BROWDOWNLEFT,
    BROWDOWNRIGHT,
    BROWINNERUP,
    BROWOUTERUPLEFT,
    BROWOUTERUPRIGHT,
    CHEEKPUFF,
    CHEEKSQUINTLEFT,
    CHEEKSQUINTRIGHT,
    NOSESNEERLEFT,
    NOSESNEERRIGHT,
    TONGUEOUT,
    COUNT
};





struct FActorMeta
{
    bool hasGloves = false;
    bool hasLeftGlove = false;
    bool hasRightGlove = false;
    bool hasBody = false;
    bool hasFace = false;
};


struct FTransform
{
    QVector3D position;
    QQuaternion rotation;
};


struct FPropSnapshot
{
    QString name = "";
    QString id = "";
    QColor color = Qt::black;
    int type = 0;
    QVector3D position = QVector3D();
    QQuaternion rotation = QQuaternion();
    std::map<QString, QString> meta;  // arbitrary key/values
};


struct FTrackerSnapshot
{
    QString name = "";
    QString connectionId = "";
    QVector3D position = QVector3D();
    QQuaternion rotation = QQuaternion();
    bool isLive = false;
    int trackingResult = 0;
    int trackerType = 0;
    QString RenderModelName = "";
    int battery = 0;
};


struct FActorDimensions
{
    float totalHeight = 0.0;
    float hipHeight = 0.0;
};


struct FFaceSnapshot
{
    QString type = "";
    int version = 0;
    double timestamp = 0.0;
    QString provider = "";
    QString faceId = "";
    QString profileName = "";
    std::map<QString, float> shapes;
};


struct FActorSnapshot
{
    QString name = "";
    QString id = "";
    QString profileName = "";
    QColor color = Qt::black;
    FActorMeta meta {};
    FActorDimensions dimentions;
    std::map<QString, FTransform> body;
    FFaceSnapshot face;
};
