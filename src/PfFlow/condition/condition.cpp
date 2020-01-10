#include "condition.h"

#include "data.h"

#include "../../PfCommon/tools/ut_error.h"

Condition::Condition()
    :mResult(false),
      strV(""),
      strCalc("")
{
}

void Condition::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;
    const char *point = nullptr;

    pTmpElem = xmlEle->FirstChildElement("v");
    if (pTmpElem)
    {
        point = pTmpElem->GetText();
        if(point)
            strV = std::string(point);
    }

    pTmpElem = xmlEle->FirstChildElement("Calc");
    if (pTmpElem)
    {
        point = pTmpElem->GetText();
        if(point)
            strCalc = pTmpElem->GetText();
    }

    ///校验 v 格式

    std::string lev1, lev2, lev3, var, v;
    if(!data::getIndexFromV(strV, lev1, lev2, lev3, var, v))
    {
        dataValueType value = 0;
        if(!data::converDigital(strV, value))
        {
            THROW_EXCEPTION("V(" + strV + ")格式异常!!");
        }
    }

    ///校验Calc格式
    if(!data::isBeyondSymboolOk(strCalc))
    {
        THROW_EXCEPTION("Calc(" + strCalc + ")格式异常!!");
    }
}
bool Condition::exec()
{
    dataValueType value = 0;

    mResult = false;

    ///获取待比较值
    std::string lev1, lev2, lev3, var, v;
    if(data::getIndexFromV(strV, lev1, lev2, lev3, var, v))
    {
        ///获取obj
        runObj *obj = nullptr;
        obj = mRunobj->getObj(lev1, lev2, lev3);
        if(obj)
        {
            std::string tmp = obj->getVarValue(var);
            data::converDigital(tmp, value);
        }
        else
        {
            UT_SHOW("get obj faild(" + lev1 + "," + lev2 + "," + lev3 + ")");
        }
    }
    else
    {
        data::converDigital(strV, value);
    }

    ///比较
    if(calcFactory::calc(strCalc, value))
        mResult = true;

    return mResult;
}

std::string Condition::getVarValue(runObj *mRunObj, std::string use)
{
    ///获取其它测试步骤值
    std::string lev1, lev2, lev3, var, v;
    if(data::getIndexFromV(use, lev1, lev2, lev3, var, v))
    {
        ///获取obj
        runObj *obj = nullptr;
        obj = mRunObj->getObj(lev1, lev2, lev3);
        if(obj)
        {
            return obj->getVarValue(var);
        }
        else
        {
#ifndef QT_NO_DEBUG
            UT_SHOW("get obj faild(" + lev1 + "," + lev2 + "," + lev3 + ")");
#endif
        }
    }

    return "";
}
