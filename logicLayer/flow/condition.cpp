#include "condition.h"
#include "../src/PfCommon/tools/ut_error.h"
#include "../src/PfSql/paramsTable/paramstable.h"

#include "../virtualParams/virtualparams.h"

#include "frame.h"

void startCondition::init(TiXmlElement *xmlEle)
{
    TiXmlElement *ele = nullptr;

    const char *point = nullptr;

    ele = xmlEle->FirstChildElement("system_uuid");
    if(ele)
    {
        point = ele->GetText();
        if(point)
            mId = std::string(point);
    }

    ele = xmlEle->FirstChildElement("table");
    if(ele)
    {
        point = ele->GetText();
        if(point)
        {
            mTable = std::string(point);

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
            mCoding = std::string(point);
    }
}

Json::Value startCondition::getRunItems()
{
    Json::Value js;

    //根据表号编码获取名称
    Json::Value paramValue;

    if(paramsTable::getInstance()->getValue(mTable, mCoding, paramValue))
        js["trigger"] = paramValue[PARAM_TABLE_PARAM_NAME].asString();
    else
        js["trigger"] = "";

    return js;
}

bool startCondition::isConform(const std::string &uuid, const std::string &table, const std::string &coding)
{
    bool res = true;

    res = virtualParams::getInstance()->isMeet({uuid, table, coding});

    return res;
}

bool startCondition::isConform()
{
    bool res = true;

    res = virtualParams::getInstance()->isMeet({mId, mTable, mCoding});

    return res;
}

/******************************************/


void stopCondition::init(TiXmlElement *xmlEle)
{
    TiXmlElement *ele = nullptr;

    const char *point = nullptr;

    ele = xmlEle->FirstChildElement("system_uuid");
    if(ele)
    {
        point = ele->GetText();
        if(point)
            mId = std::string(point);
    }

    ele = xmlEle->FirstChildElement("table");
    if(ele)
    {
        point = ele->GetText();
        if(point)
            mTable = std::string(point);
    }

    ele = xmlEle->FirstChildElement("coding");
    if(ele)
    {
        point = ele->GetText();
        if(point)
            mCoding = std::string(point);
    }
}

bool stopCondition::isConform()
{
    bool res = true;

    res = virtualParams::getInstance()->isMeet({mId, mTable, mCoding});

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


