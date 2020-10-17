#include "jsonv3parser.h"
#include "lz4frame.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "animations.h"
#include "mapping.h"


JsonV3Parser::JsonV3Parser()
{

}

void JsonV3Parser::feed(QByteArray &datagram, qint64 readLen)
{
    // decompress json3 and build frame data json object
    constexpr int bufferSize{ 8192 * 32 };
    if (!mDecompressionBuffer)
    {
        mDecompressionBuffer = new char[bufferSize];
        // fill buffer with zeros. To avoid problem when garbage contains '}'
        // which lead to json parse error
        std::fill(mDecompressionBuffer, mDecompressionBuffer + bufferSize, 0);
    }

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

//     printf("\n\n%s\n\n", mDecompressionBuffer);

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
    foreach(const QJsonValue& value, propsArray)
    {
        QJsonObject prop = value.toObject();
        FPropSnapshot propSnapshot;
        propSnapshot.name = prop["name"].toString();
        propSnapshot.id = prop["name"].toString();

        auto propColor = prop["color"].toArray();
        propSnapshot.color = QColor(propColor[0].toDouble(), propColor[1].toDouble(), propColor[2].toDouble());

        propSnapshot.type = prop["type"].toInt();

        auto propPostiion = prop["position"].toObject();
        propSnapshot.position = QVector3D(propPostiion["x"].toDouble(), propPostiion["y"].toDouble(), propPostiion["z"].toDouble());

        auto propRotation = prop["rotation"].toObject();
        propSnapshot.rotation = QQuaternion(propRotation["w"].toDouble(), propRotation["x"].toDouble(), propRotation["y"].toDouble(), propRotation["z"].toDouble());

        propsMap[prop["name"].toString()] = propSnapshot;
    }
    Animations::get()->putProps(propsMap);

    // parse actors
    QHash<QString, FActorSnapshot> actorsMap;
    QHash<QString, FFaceSnapshot> facesMap;

    QJsonArray actorsArray = sceneObject["actors"].toArray();
    foreach(const QJsonValue& value, actorsArray)
    {
        QJsonObject actor = value.toObject();
        // get meta
        QJsonObject actorMeta = actor["meta"].toObject();
        FActorSnapshot actorSnapshot;

        if(actorMeta["hasBody"].toBool() || actorMeta["hasGloves"].toBool())
        {
            // put body
            actorSnapshot.name = actor["name"].toString();
            actorSnapshot.id = actor["name"].toString();
            actorSnapshot.profileName = actor["name"].toString();

            auto actorBody = actor["body"].toObject();

            foreach(QString key, actorBody.keys())
            {
                if (actorBody[key].isObject())
                {
                    QJsonObject valueObject = actorBody[key].toObject();
                    if(valueObject.contains("position") && valueObject.contains("rotation"))
                    {
                        auto position = valueObject["position"].toObject();
                        auto rotation = valueObject["rotation"].toObject();

                        actorSnapshot.body[key] = FTransform {
                                QVector3D(position["x"].toDouble(), position["y"].toDouble(), position["z"].toDouble()),
                                QQuaternion(rotation["w"].toDouble(), rotation["x"].toDouble(), rotation["y"].toDouble(), rotation["z"].toDouble())
                        };
                    }
                }
            }

            actorsMap[actorSnapshot.id] = actorSnapshot;
        }

        // put face
        if(actorMeta["hasFace"].toBool())
        {
            QJsonObject faceObject = actor["face"].toObject();

            FFaceSnapshot faceSnapshot;
            faceSnapshot.type = faceObject["type"].toString("");
            faceSnapshot.version = faceObject["version"].toInt(0);
            faceSnapshot.timestamp = faceObject["timestamp"].toDouble(0.0);
            faceSnapshot.provider = faceObject["provider"].toString("");
            faceSnapshot.faceId = faceObject["faceId"].toString("");
            faceSnapshot.profileName = actorSnapshot.profileName;
            faceSnapshot.parentActorId = actorSnapshot.id;

            const QStringList studioShapeNames = Mapping::get()->getFaceShapeNames();
            foreach(QString key, faceObject.keys())
            {
                if(studioShapeNames.contains(key))
                    faceSnapshot.shapes[key] = faceObject[key].toDouble();
            }

            facesMap[faceSnapshot.faceId] = faceSnapshot;
        }
    }

    Animations::get()->putActors(actorsMap);
    Animations::get()->putFaces(facesMap);

}

void JsonV3Parser::apply()
{
    Animations::get()->applyAnimationsToMappedObjects();
}

std::vector<FPropSnapshot> JsonV3Parser::propSnapshots()
{
    std::vector<FPropSnapshot> result;

    QList<FPropSnapshot> ls = Animations::get()->getProps().values();
    std::copy(ls.begin(), ls.end(), std::back_inserter(result));

    return result;
}

std::vector<FActorSnapshot> JsonV3Parser::actorSnapshots()
{
    std::vector<FActorSnapshot> result;
    for (auto snapshot : Animations::get()->getActors().values().toStdList())
        result.push_back(snapshot);
    return result;
}
