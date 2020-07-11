#include "beargextract.h"

#include <QDebug>

beArgExtract::beArgExtract(QObject *parent):
    argExtract(parent)
{

}

beArgExtract::~beArgExtract()
{

}

void beArgExtract::extract(const QString &uuid, const Json::Value &otherParam, std::shared_ptr<PfIcdWorkBench::frameObj> frameObj, const Json::Value &result)
{
    //更新信息字格式1、3参数
    Json::Value newJs;
    int infoType = -1;
    if(frameObj->getValidValue(result, newJs, infoType))
    {
        if(infoType != 1)
        {
            for(int index = 0; index < newJs.size(); index++)
            {
                int coding = 0, table = 0;
                Json::Value value = Json::Value();
                if(!newJs[index]["coding"].isNull())
                {
                    coding = newJs[index]["coding"].asInt();
                }

                if(!newJs[index]["table"].isNull())
                {
                    table = newJs[index]["table"].asInt();
                }

                if(!newJs[index]["value"].isNull())
                {
                    value = newJs[index]["value"];
                }

    #ifndef QT_NO_DEBUG
                qDebug() << "[BE]->" << QString::number(table) << "," << QString::number(coding) << "," << value.asString().c_str();
    #endif
            }
        }
    }

    //更新信息字格式2参数
    if(1 == infoType)
    {
        if(!result["region"].isNull())
        {
            Json::Value regionValue = result["region"];

            if(!regionValue["table_num"].isNull())
            {
                unsigned int table = regionValue["table_num"].asUInt();

                Json::Value array = regionValue["data"];
                for(int index = 0; index < array.size(); index++)
                {
                    int coding = array[index]["coding"].asInt();

    #ifndef QT_NO_DEBUG
                    qDebug() << "[BE]->" << QString::number(table, 10) << "," << QString::number(coding) << "," << array[index]["value"].asString().c_str();
    #endif
                }
            }
        }
    }
}
