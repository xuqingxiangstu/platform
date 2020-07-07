#include "condition.h"
#include "../src/PfCommon/tools/ut_error.h"
#include "../src/PfSql/paramsTable/paramstable.h"



#include "frame.h"

void startCondition::init(TiXmlElement *xmlEle)
{
    mValue = Json::Value();

    TiXmlElement *ele = nullptr;

    const char *point = nullptr;

    ele = xmlEle->FirstChildElement("system_uuid");
    if(ele)
    {
        point = ele->GetText();
        if(point)
            mId = QString(point);
    }

    ele = xmlEle->FirstChildElement("table");
    if(ele)
    {
        point = ele->GetText();
        if(point)
        {
            mTable = QString(point);

            if(UI_CUSTOM_MSG == mTable) //界面内容消息触发
            {
               mId = "UI";
            }
        }
    }

    ele = xmlEle->FirstChildElement("coding");
    if(ele)
    {
        point = ele->GetText();
        if(point)
            mCoding = QString(point);
    }

    ele = xmlEle->FirstChildElement("value");
    if(ele)
    {
        TiXmlElement *valueEle = nullptr;
        std::string type = "";

        valueEle = ele->FirstChildElement("type");
        if(valueEle)
        {
            point = valueEle->GetText();
            if(point)
                type = point;
        }

        valueEle = ele->FirstChildElement("context");
        if(valueEle)
        {
            point = valueEle->GetText();
            if(point)
            {
                if("string" == type)
                {
                    mValue = Json::Value(std::string(point));
                }
                else if("double" == type)
                {
                    mValue = Json::Value(std::atof(point));
                }
                else if("int" == type)
                {
                    mValue = Json::Value(std::atoi(point));
                }
                else
                {
                    mValue = Json::Value();
                }
            }
        }
    }

    mConditionKey.first = mId;
    mConditionKey.second = mTable;
    mConditionKey.three = mCoding;
}

Json::Value startCondition::getRunItems()
{
    Json::Value js;

    //根据表号编码获取名称
    Json::Value paramValue;

    if(paramsTable::getInstance()->getValue(mTable, mCoding, paramValue))
    {
        //js["trigger"] = paramValue[PARAM_TABLE_PARAM_NAME].asString();
        js["trigger"] = "[" + paramValue[PARAM_TABLE_TABLE_NUM].asString() + "-" + paramValue[PARAM_TABLE_CODING_NUM].asString() + "]"+ paramValue[PARAM_TABLE_PARAM_NAME].asString();
    }
    else
        js["trigger"] = "";

    return js;
}


bool startCondition::isConform()
{
    bool res = false;

    res = virtualParams::getInstance()->isMeet(mConditionKey, mValue);

    return res;
}

/******************************************/


void stopCondition::init(TiXmlElement *xmlEle)
{
    mValue = Json::Value();
    TiXmlElement *ele = nullptr;

    const char *point = nullptr;

    ele = xmlEle->FirstChildElement("system_uuid");
    if(ele)
    {
        point = ele->GetText();
        if(point)
            mId = QString(point);
    }

    ele = xmlEle->FirstChildElement("table");
    if(ele)
    {
        point = ele->GetText();
        if(point)
            mTable = QString(point);
    }

    ele = xmlEle->FirstChildElement("coding");
    if(ele)
    {
        point = ele->GetText();
        if(point){
            mCoding = QString(point);

            //2020-5-23 ygt add
            if(UI_CUSTOM_MSG == mTable) //界面内容消息触发
            {
               mId = "UI";
            }
        }
    }

    ele = xmlEle->FirstChildElement("value");
    if(ele)
    {
        TiXmlElement *valueEle = nullptr;
        std::string type = "";

        valueEle = ele->FirstChildElement("type");
        if(valueEle)
        {
            point = valueEle->GetText();
            if(point)
                type = point;
        }

        valueEle = ele->FirstChildElement("context");
        if(valueEle)
        {
            point = valueEle->GetText();
            if(point)
            {
                if("string" == type)
                {
                    mValue = Json::Value(std::string(point));
                }
                else if("double" == type)
                {
                    mValue = Json::Value(std::atof(point));
                }
                else if("int" == type)
                {
                    mValue = Json::Value(std::atoi(point));
                }
                else
                {
                    mValue = Json::Value();
                }
            }
        }
    }

    mConditionKey.first = mId;
    mConditionKey.second = mTable;
    mConditionKey.three = mCoding;
}

bool stopCondition::isConform()
{
    bool res = true;

    res = virtualParams::getInstance()->isMeet(mConditionKey, mValue);

    return res;
}

/******************************************/


void timing::init(TiXmlElement *xmlEle)
{
    const char *point = xmlEle->GetText();
    if(std::string(point) == "CYCLE")
    {
        mTimingType = CYCLE;
    }
    else if(std::string(point) == "NOW")
    {
        mTimingType = NOW;
    }
    else if(std::string(point) == "MANUAL")
    {
        mTimingType = MANUAL;
    }
    else
    {
        UT_THROW_EXCEPTION("timing type error (" + std::string(point) + ")");
    }
}


