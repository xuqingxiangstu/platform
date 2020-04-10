#include "variateStep.h"

#include "../../PfCommon/tools/ut_error.h"

#include "../condition/condition.h"
variateStep::variateStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    strValue(),
    mResult(false)
{
}

variateStep::~variateStep()
{

}

void variateStep::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;
   ///获取变量的类型
    pTmpElem = xmlEle->FirstChildElement("ValueType");
   if(pTmpElem)
   {
     const char *point = nullptr;
     point = pTmpElem->GetText();
     if(point)
         mstrValueType = std::string(point);
   }
   ///获取变量的内容
   pTmpElem = xmlEle->FirstChildElement("Value");
    if (pTmpElem)
    {
         const char *point = nullptr;
         point = pTmpElem->GetText();
         if(point)
            strValue =std::string(point);
     }

}

std::string variateStep::getClassName()
{
    return VAR_NAME(variateStep);
}

bool variateStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[variateStep]");
#endif
    //判断是否为引用
    if((strValue.find("!") != std::string::npos)||(strValue.find("@") != std::string::npos))
    {
        mvarValue = getVarV(strValue);
    }
    else
    {
        mvarValue = strValue;
     }
    mResult=true;
    return mResult;
}
std::string variateStep::getVarValue(std::string var)
{
    std::string res = "";
    if(var == "Value")
    {
        res = mvarValue;
    }
    return res;
}
std::string variateStep::getVarV(std::string use)
{
    Condition cond;
    return cond.getVarValue(mRunObj,use);
}
bool variateStep::getResult()
{
    return mResult;
}

std::string variateStep::outText()
{
    return strOutText;
}

void variateStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void variateStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new variateStep();
    }
}

    
    
