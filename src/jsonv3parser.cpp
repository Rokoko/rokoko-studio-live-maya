#include "jsonv3parser.h"
#include "lz4frame.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "animations.h"


JsonV3Parser::JsonV3Parser()
{

}

void JsonV3Parser::feed(QByteArray &datagram, qint64 readLen)
{
    // decompress json3 and build frame data json object
    constexpr int bufferSize{ 8192 * 32 };
    if (!mDecompressionBuffer)
        mDecompressionBuffer = new char[bufferSize];

    if (!mDecompressionContext)
        LZ4F_createDecompressionContext(&mDecompressionContext, LZ4F_VERSION);

    LZ4F_resetDecompressionContext(mDecompressionContext);
    size_t srcLen = static_cast<size_t>(readLen);
    size_t dstLen = static_cast<size_t>(bufferSize);
    size_t status = LZ4F_decompress(mDecompressionContext, mDecompressionBuffer, &dstLen, datagram.data(), &srcLen, nullptr);

    // cut garbage from end
    for (size_t i = dstLen; i > 0; --i)
    {
        if (mDecompressionBuffer[i] == '}')
            break;
        mDecompressionBuffer[i] = '\0';
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(mDecompressionBuffer, &err);
    mMessageJson = doc.object();
    if(err.error != QJsonParseError::NoError)
    {
        printf("Json parse error %s\n", err.errorString().toStdString().c_str());
    }

     // printf("\n\n%s\n\n", mDecompressionBuffer);

    if(mDecompressionBuffer != nullptr)
    {
        delete [] mDecompressionBuffer;
        mDecompressionBuffer = nullptr;
    }

}

void JsonV3Parser::parse()
{

    // put timestamps
    QJsonObject sceneObject = mMessageJson["scene"].toObject();
    double timeStamp = sceneObject["timestamp"].toDouble();
    Animations::get()->timestamp = timeStamp;
    Animations::get()->playbackTimestamp = timeStamp;

    // parse props
    QJsonArray propsArray = sceneObject["props"].toArray();
    QHash<QString, FPropSnapshot> propsMap;
    foreach(const QJsonValue& value, propsArray) {
        QJsonObject prop = value.toObject();
        FPropSnapshot propSnapshot;
        propSnapshot.name = prop["name"].toString();

        auto propColor = prop["color"].toArray();
        propSnapshot.color = QColor(propColor[0].toDouble(), propColor[1].toDouble(), propColor[2].toDouble());

        auto propPostiion = prop["position"].toArray();
        propSnapshot.position = QVector3D(propPostiion[0].toDouble(), propPostiion[1].toDouble(), propPostiion[2].toDouble());

        auto propRotation = prop["rotation"].toArray();
        propSnapshot.rotation = QQuaternion(propRotation[0].toDouble(), propRotation[1].toDouble(), propRotation[2].toDouble(), propRotation[3].toDouble());

        propsMap[prop["name"].toString()] = propSnapshot;
    }
    Animations::get()->putProps(propsMap);

    // parse actors
    QHash<QString, FActorSnapshot> actorsMap;
    QHash<QString, QJsonObject> facesMap;

    QJsonArray actorsArray = sceneObject["actors"].toArray();
    foreach(const QJsonValue& value, actorsArray) {
        QJsonObject actor = value.toObject();
        // get meta
//        QJsonObject actorMeta = actor["meta"].toObject();

//        if(actorMeta["hasBody"].toBool())
//        {
//            // save bodies
//            actorsMap[actor["name"].toString()] = actor["body"].toObject();
//        }
//        if(actorMeta["hasFace"].toBool())
//        {
//            // save face
//            QJsonObject actorFace = actor["face"].toObject();
//            facesMap[actorFace["faceId"].toString()] = actorFace;
//        }
    }
    Animations::get()->putActors(actorsMap);
    // Animations::get()->putFaces(facesMap);

}

void JsonV3Parser::apply()
{
    Animations::get()->applyAnimationsToMappedObjects();
}

std::vector<FPropSnapshot> JsonV3Parser::propSnapshots()
{
    std::vector<FPropSnapshot> result;

    QJsonObject sceneObject = mMessageJson["scene"].toObject();

    foreach(auto prop, sceneObject["props"].toArray())
    {
        QJsonObject propObject = prop.toObject();

        FPropSnapshot snapshot;
        snapshot.name = propObject["name"].toString();
        auto color = propObject["color"].toArray();
        snapshot.color = QColor::fromRgbF(color[0].toDouble(), color[1].toDouble(), color[2].toDouble());
        snapshot.type = propObject["type"].toInt();

        auto position = propObject["position"].toArray();
        snapshot.position = QVector3D(position[0].toDouble(), position[1].toDouble(), position[2].toDouble());

        auto rotation = propObject["rotation"].toArray();
        snapshot.rotation = QQuaternion(rotation[0].toDouble(), rotation[1].toDouble(), rotation[2].toDouble(), rotation[3].toDouble());

        result.push_back(snapshot);
    }

    return result;
}

std::vector<FActorSnapshot> JsonV3Parser::actorSnapshots()
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
