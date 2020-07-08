#include "middleargextract.h"

#include "../src/PfSql/paramsTable/paramstable.h"
#include "../src/PfCommon/tools/ut_error.h"
#include <QDebug>
#include <QString>

middleArgExtract::middleArgExtract(QObject *parent):
    argExtract(parent)
{

}

middleArgExtract::~middleArgExtract()
{

}

void middleArgExtract::extract(const Json::Value &otherParam, std::shared_ptr<PfIcdWorkBench::frameObj> frameObj, const Json::Value &result)
{
    //更新参数
    Json::Value newJs;
    int infoType = -1;

    mOtherParam = otherParam;

    if(frameObj->getValidValue(result, newJs, infoType))
    {
        if(infoType != 1)
        {
            for(int index = 0; index < newJs.size(); index++)
            {
                Json::Value value = Json::Value();
                std::string srcValue = "";
                int coding = 0, table = 0;
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
                if(!newJs[index]["src_value"].isNull())
                {
                    srcValue = newJs[index]["src_value"].asString();
                }
                else
                {
                    continue;
                }

                judge(table, coding, value,srcValue);
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
                    std::string srcValue = "";
                    srcValue = array[index]["src_value"].asString();

                    judge(table, coding, array[index]["value"], srcValue);
                }
            }
        }
    }
}

void middleArgExtract::judge(const unsigned int &table, const unsigned int &coding, const Json::Value &value, const std::string &srcValue)
{
    bool findMean = false;
    std::string chMean = "";

    bool isOver = false;

    Json::Value confJs;
    if(paramsTable::getInstance()->getValue(QString::number(table, 10), coding, confJs))
    {
        //step1:判断是否超差

        std::string minStr = confJs[PARAM_TABLE_MIN_VALUE].asString();
        std::string maxStr = confJs[PARAM_TABLE_MAX_VALUE].asString();

        if(minStr == "" && maxStr == "")
        {
            //不进行判断
        }
        else
        {
            double minV = QString::fromStdString(minStr).toDouble();
            double maxV = QString::fromStdString(maxStr).toDouble();

            if(value.isInt())
            {
                int curV = value.asInt();
                if( !(curV >= minV && curV <= maxV) )
                    isOver = true;
            }
            else if(value.isDouble())
            {
                double curV = value.asDouble();
                if( !(curV >= minV && curV <= maxV) )
                    isOver = true;
            }
        }

        //step2：获取物理含义
        QString mean = QString::fromStdString(confJs[PARAM_TABLE_VALUE_MEAN].asString());
        if(mean.compare("") == 0)
        {

        }
        else
        {
            QStringList res = mean.split(";");
            for(QString v : res)
            {
                QStringList tmpList = v.split(":");
                if(tmpList.size() > 1)
                {
                    QString initValue = tmpList.at(0);

                    bool isOk = false;
                    //判断类型
                    std::string dataType = confJs[PARAM_TABLE_DATA_TYPE].asString();

                    if( (DATA_TYPE_NCHAR == dataType) || (DATA_TYPE_NRAW == dataType))
                    {

                    }
                    else if( (DATA_TYPE_IEEE32 == dataType) || (DATA_TYPE_IEEE64 == dataType))
                    {
                        double tmpV = initValue.toDouble(&isOk);
                        if(isOk)
                        {
                            if(qFuzzyCompare(value.asDouble(), tmpV))
                            {
                                findMean = true;
                                chMean = tmpList.at(1).toStdString();
                                break;
                            }
                        }
                    }
                    else
                    {
                        unsigned int tmpV = initValue.toInt(&isOk, 10);

                        if(!isOk)
                            tmpV = initValue.toUInt(&isOk, 16);

                        if(!isOk)
                            tmpV = 0;

                        if(tmpV == value.asInt())
                        {
                            findMean = true;
                            chMean = tmpList.at(1).toStdString();
                            break;
                        }
                    }
                }
            }
        }

    }

#ifdef PRINT_RESULT

    QString overStr = "";
    isOver == true ? overStr = "异常" : overStr = "正常";
#ifndef QT_NO_DEBUG
    qDebug() << "[middle]->" << QString::number(table, 10) << "," << QString::number(coding) << "," << srcValue.c_str() << "," << value.asString().c_str()
             << "," ;
    SHOW(chMean + "," + overStr.toStdString());
#endif

#endif
}
