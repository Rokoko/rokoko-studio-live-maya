#include "jsonv2parser.h"
#include "animations.h"
#include "mapping.h"
#include <QJsonDocument>
#include <QJsonArray>


JsonV2Parser::JsonV2Parser()
{

}

void JsonV2Parser::feed(QByteArray& datagram, qint64 readLen)
{
    Q_UNUSED(readLen)

    mDatagram = datagram;

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(datagram.data(), &err);
    if(err.error != QJsonParseError::NoError)
    {
        printf("Json parse error: %s\n", err.errorString().toStdString().c_str());
    }
    mMessageJson = doc.object();
}

void JsonV2Parser::parse()
{
    // put timestamps
    Animations::get()->timestamp = mMessageJson["timestamp"].toDouble();
    Animations::get()->playbackTimestamp = mMessageJson["playbackTimestamp"].toDouble();

    // parse props
    QJsonArray propsArray = mMessageJson["props"].toArray();

    QHash<QString, FPropSnapshot> propsMap;
    foreach(const QJsonValue& value, propsArray) {
        QJsonObject prop = value.toObject();

        FPropSnapshot snapshot;
        snapshot.name = prop["name"].toString();
        snapshot.meta["id"] = prop["id"].toString();

        auto propPostiion = prop["position"].toObject();
        snapshot.position = QVector3D(propPostiion["x"].toDouble(), propPostiion["y"].toDouble(), propPostiion["z"].toDouble());

        auto propRotation = prop["rotation"].toObject();
        snapshot.rotation = QQuaternion(propRotation["w"].toDouble(), propRotation["x"].toDouble(), propRotation["y"].toDouble(), propRotation["z"].toDouble());

        propsMap[prop["id"].toString()] = snapshot;

    }
    Animations::get()->putProps(propsMap);

    // parse trackers
    QHash<QString, FTrackerSnapshot> trackersMap;
    QJsonArray trackersArray = mMessageJson["trackers"].toArray();
    foreach(const QJsonValue& value, trackersArray) {
        FTrackerSnapshot snapshot;
        QJsonObject trackerObject = value.toObject();

        snapshot.name = trackerObject["name"].toString();

        auto propPostiion = trackerObject["position"].toArray();
        snapshot.position = QVector3D(propPostiion[0].toDouble(), propPostiion[1].toDouble(), propPostiion[2].toDouble());

        auto propRotation = trackerObject["rotation"].toArray();
        snapshot.rotation = QQuaternion(propRotation[0].toDouble(), propRotation[1].toDouble(), propRotation[2].toDouble(), propRotation[3].toDouble());

        trackersMap[trackerObject["id"].toString()] = snapshot;
    }
    Animations::get()->putTrackers(trackersMap);

    // put faces
    QHash<QString, FFaceSnapshot> facesMap;
    QJsonArray faceArray = mMessageJson["faces"].toArray();
    foreach(auto face, faceArray)
    {
        QJsonObject faceObject = face.toObject();

        FFaceSnapshot snapshot;
        snapshot.type = faceObject["type"].toString("");
        snapshot.version = faceObject["version"].toInt(0);
        snapshot.timestamp = faceObject["timestamp"].toDouble(0.0);
        snapshot.provider = faceObject["provider"].toString("");
        snapshot.faceId = faceObject["faceId"].toString("");
        snapshot.profileName = faceObject["profileName"].toString("");

        const QStringList studioShapeNames = Mapping::get()->getFaceShapeNames();
        foreach(QString key, faceObject.keys())
        {
            if(studioShapeNames.contains(key))
                snapshot.shapes[key] = faceObject[key].toDouble();
        }

        facesMap[faceObject["faceId"].toString()] = snapshot;
    }
    Animations::get()->putFaces(facesMap);

    // parse actors
    QHash<QString, FActorSnapshot> actorsMap;
    QJsonArray actorsArray = mMessageJson["actors"].toArray();
    foreach(const QJsonValue& value, actorsArray) {
        QJsonObject actor = value.toObject();
        FActorSnapshot snapshot;

        snapshot.name = actor["name"].toString();
        snapshot.id = actor["id"].toString();
        snapshot.profileName = actor["profileName"].toString();
        // put body
        foreach(QString key, actor.keys())
        {
            if (actor[key].isObject())
            {
                QJsonObject valueObject = actor[key].toObject();
                if(valueObject.contains("position") && valueObject.contains("rotation"))
                {
                    auto position = valueObject["position"].toObject();
                    auto rotation = valueObject["rotation"].toObject();

                    snapshot.body[key] = FTransform {
                            QVector3D(position["x"].toDouble(), position["y"].toDouble(), position["z"].toDouble()),
                            QQuaternion(rotation["w"].toDouble(), rotation["x"].toDouble(), rotation["y"].toDouble(), rotation["z"].toDouble())
                    };

                }
            }
        }

        actorsMap[snapshot.id] = snapshot;
    }
    Animations::get()->putActors(actorsMap);
}

void JsonV2Parser::apply()
{
    Animations::get()->applyAnimationsToMappedObjects();
}

std::vector<FPropSnapshot> JsonV2Parser::propSnapshots()
{
    std::vector<FPropSnapshot> result;
    foreach(auto prop, mMessageJson["props"].toArray())
    {
        QJsonObject propObject = prop.toObject();

        FPropSnapshot snapshot;
        snapshot.name = propObject["name"].toString();
        snapshot.id = propObject["id"].toString();

        auto position = propObject["position"].toArray();
        snapshot.position = QVector3D(position[0].toDouble(), position[1].toDouble(), position[2].toDouble());

        auto rotation = propObject["rotation"].toArray();
        snapshot.rotation = QQuaternion(rotation[0].toDouble(), rotation[1].toDouble(), rotation[2].toDouble(), rotation[3].toDouble());

        result.push_back(snapshot);

    }

    return result;
}

std::vector<FActorSnapshot> JsonV2Parser::actorSnapshots()
{
    QJsonArray actorsArray = mMessageJson["actors"].toArray();
    std::vector<FActorSnapshot> result;
    foreach(auto actor, actorsArray)
    {
        QJsonObject actorObject = actor.toObject();

        FActorSnapshot snapshot;
        snapshot.name = actorObject["name"].toString();
        snapshot.profileName = actorObject["profileName"].toString();
        result.push_back(snapshot);
    }
    return result;
}
