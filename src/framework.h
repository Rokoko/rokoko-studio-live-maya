#pragma once

#include <QString>
#include <QColor>
#include <QVector3D>
#include <QQuaternion>
#include <vector>
#include <map>


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
    QString parentActorId = "";
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
