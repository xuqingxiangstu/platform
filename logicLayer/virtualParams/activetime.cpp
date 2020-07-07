#include "activetime.h"

#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

std::shared_ptr<activeTime> activeTime::mInstance = nullptr;

activeTime::activeTime()
{
    QFile loadFile(mFilePath);
    if(!loadFile.open(QIODevice::ReadWrite))
    {
        throw std::runtime_error("[ERROR] open file faild => " + mFilePath.toStdString());
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        throw std::runtime_error("[ERROR] parse json error");
    }

    QJsonObject rootObj = jsonDoc.object();

    if(!rootObj.contains("active"))
    {
        throw std::runtime_error("[ERROR] no configure key");
    }

    mActiveTime = rootObj.value("active").toInt();
}

activeTime::~activeTime()
{

}
