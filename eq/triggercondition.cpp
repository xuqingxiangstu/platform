#include "triggercondition.h"

#include "../src/PfSql/paramsTable/paramstable.h"
#include "../src/PfSql/paramsTable/systemtable.h"
#include "../src/PfSql/paramsTable/sysinterfacetable.h"

#include <QMessageBox>
#include "dragTree/dragrole.h"
#include "./property/templateproperty.h"
#include "./property/nodeproperty.h"

std::shared_ptr<triggerCondition> triggerCondition::mInstance = nullptr;

QStringList triggerCondition::getValueMeans(QString value)
{
    QStringList meansList;

    if(value.compare("") != 0)
    {
        meansList = value.split(";");

        //qDebug() << meansList;
    }

    return meansList;
}


triggerCondition::triggerCondition()
{
    Json::Value cmdJs;

    Json::Value noJs;
    noJs[PROPERTY_CONDITION_VALUE_NAME] = PROPERTY_CONDITION_NO;
    noJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = -1;
    noJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = -1;
    noJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value();

    mConditionInitValue.append(noJs);

    if(paramsTable::getInstance()->getCmdValues(cmdJs))
    {
        for(int index = 0; index < cmdJs.size(); index++)
        {
            Json::Value tmpJs;

            tmpJs[PROPERTY_CONDITION_VALUE_NAME] = "[指令][" + cmdJs[index][PARAM_TABLE_TABLE_NUM].asString() + "-" + cmdJs[index][PARAM_TABLE_CODING_NUM].asString() + "]"+ cmdJs[index][PARAM_TABLE_PARAM_NAME].asString();
            tmpJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = cmdJs[index][PARAM_TABLE_TABLE_NUM];
            tmpJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = cmdJs[index][PARAM_TABLE_CODING_NUM];
            tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value();

            mConditionInitValue.append(tmpJs);
        }
    }

    //modify xqx 2020-6-7 09:57:48 增加参数触发条件，增加值判断，如果是指令则值为空，其它为实际值

    Json::Value paramsJs;

    if(paramsTable::getInstance()->getParamValues(paramsJs))
    {
        for(int index = 0; index < paramsJs.size(); index++)
        {
            Json::Value tmpJs;

            QString meanValue = QString::fromStdString(paramsJs[index][PARAM_TABLE_VALUE_MEAN].asString());

            QStringList means = getValueMeans(meanValue);

            if(!(means.size() > 0))   //为空时代表只要收到数据就触发
            {
                tmpJs[PROPERTY_CONDITION_VALUE_NAME] = "[数值][" + paramsJs[index][PARAM_TABLE_TABLE_NUM].asString() + "-" + paramsJs[index][PARAM_TABLE_CODING_NUM].asString()+ "]"+ paramsJs[index][PARAM_TABLE_PARAM_NAME].asString();
                tmpJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = paramsJs[index][PARAM_TABLE_TABLE_NUM];
                tmpJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = paramsJs[index][PARAM_TABLE_CODING_NUM];
                tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value();

                mConditionInitValue.append(tmpJs);
            }
            else
            {
                for(QString mean : means)
                {
                    if(mean == "")
                        continue;

                    tmpJs[PROPERTY_CONDITION_VALUE_NAME] = "[数值][" + paramsJs[index][PARAM_TABLE_TABLE_NUM].asString()
                            + "-" + paramsJs[index][PARAM_TABLE_CODING_NUM].asString()+ "]"
                            + paramsJs[index][PARAM_TABLE_PARAM_NAME].asString()
                            + " => " + mean.toStdString();

                    tmpJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = paramsJs[index][PARAM_TABLE_TABLE_NUM];
                    tmpJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = paramsJs[index][PARAM_TABLE_CODING_NUM];

                    QStringList tmpList = mean.split(":");
                    if(tmpList.size() > 0)
                    {
                        QString initValue = tmpList.at(0);

                        bool isOk = false;
                        //判断类型
                        std::string dataType = paramsJs[index][PARAM_TABLE_DATA_TYPE].asString();

                        if( (DATA_TYPE_NCHAR == dataType) || (DATA_TYPE_NRAW == dataType))
                        {
                            tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value(initValue.toStdString());
                        }
                        else if( (DATA_TYPE_IEEE32 == dataType) || (DATA_TYPE_IEEE64 == dataType))
                        {
                            double tmpV = initValue.toDouble(&isOk);
                            if(isOk)
                                tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value(tmpV);
                            else
                                tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value();
                        }
                        else
                        {
                            unsigned int tmpV = initValue.toInt(&isOk, 10);

                            if(!isOk)
                                tmpV = initValue.toUInt(&isOk, 16);

                            if(!isOk)
                                tmpV = 0;

                            tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value(tmpV);
                        }

                        mConditionInitValue.append(tmpJs);
                    }
                }
            }
        }
    }

    Json::Value stateJs;

    if(paramsTable::getInstance()->getStateValues(stateJs))
    {
        for(int index = 0; index < stateJs.size(); index++)
        {
            Json::Value tmpJs;

            QString meanValue = QString::fromStdString(stateJs[index][PARAM_TABLE_VALUE_MEAN].asString());

            QStringList means = getValueMeans(meanValue);

            if(!(means.size() > 0))   //为空时代表只要收到数据就触发
            {
                tmpJs[PROPERTY_CONDITION_VALUE_NAME] = "[状态][" + stateJs[index][PARAM_TABLE_TABLE_NUM].asString() + "-" + paramsJs[index][PARAM_TABLE_CODING_NUM].asString()+ "]"+ paramsJs[index][PARAM_TABLE_PARAM_NAME].asString();
                tmpJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = stateJs[index][PARAM_TABLE_TABLE_NUM];
                tmpJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = stateJs[index][PARAM_TABLE_CODING_NUM];
                tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value();

                mConditionInitValue.append(tmpJs);
            }
            else
            {
                for(QString mean : means)
                {
                    if(mean == "")
                        continue;

                    tmpJs[PROPERTY_CONDITION_VALUE_NAME] = "[状态][" + stateJs[index][PARAM_TABLE_TABLE_NUM].asString()
                            + "-" + stateJs[index][PARAM_TABLE_CODING_NUM].asString()+ "]"
                            + stateJs[index][PARAM_TABLE_PARAM_NAME].asString()
                            + " => " + mean.toStdString();

                    tmpJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = stateJs[index][PARAM_TABLE_TABLE_NUM];
                    tmpJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = stateJs[index][PARAM_TABLE_CODING_NUM];

                    QStringList tmpList = mean.split(":");
                    if(tmpList.size() > 0)
                    {
                        QString initValue = tmpList.at(0);

                        bool isOk = false;
                        //判断类型
                        std::string dataType = stateJs[index][PARAM_TABLE_DATA_TYPE].asString();

                        if( (DATA_TYPE_NCHAR == dataType) || (DATA_TYPE_NRAW == dataType))
                        {
                            tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value(initValue.toStdString());
                        }
                        else if( (DATA_TYPE_IEEE32 == dataType) || (DATA_TYPE_IEEE64 == dataType))
                        {
                            double tmpV = initValue.toDouble(&isOk);
                            if(isOk)
                                tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value(tmpV);
                            else
                                tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value();
                        }
                        else
                        {
                            unsigned int tmpV = initValue.toInt(&isOk, 10);

                            if(!isOk)
                                tmpV = initValue.toUInt(&isOk, 16);

                            if(!isOk)
                                tmpV = 0;

                            tmpJs[PROPERTY_CONDITION_VALUE_VALUE] = Json::Value(tmpV);
                        }

                        mConditionInitValue.append(tmpJs);
                    }
                }
            }
        }
    }
    //end
}

triggerCondition::~triggerCondition()
{

}

void triggerCondition::getInitValue(Json::Value &value)
{
    value = mConditionInitValue;
}
