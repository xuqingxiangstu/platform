#include "judgeStep.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../condition/condition.h"
#include "../../PfCommon/CalcFormula/calcformula.h"
judgeStep::judgeStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

judgeStep::~judgeStep()
{

}

void judgeStep::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;
   ///获取输入的变量
    pTmpElem = xmlEle->FirstChildElement("v");
   if(pTmpElem)
   {
     const char *point = nullptr;
     point = pTmpElem->GetText();
     if(point)
         mstrVContent = std::string(point);
   }
   ///表达式
   pTmpElem = xmlEle->FirstChildElement("JudgeCalc");
    if (pTmpElem)
    {
         const char *point = nullptr;
         point = pTmpElem->GetText();
         if(point)
            mstrexp =std::string(point);
     }
}

std::string judgeStep::getClassName()
{
    return VAR_NAME(judgeStep);
}

bool judgeStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[judgeStep]");
#endif
    std::string strExpress="";
    Pf::PfCommon::CalcFormula Calc;
    if(mstrexp.find("v")) //判断表达式中是否含变量v
    {
        //判断是否为引用
        if((mstrVContent.find("!") != std::string::npos)||(mstrVContent.find("@") != std::string::npos))
        {
            mstrVValue = getVarV(mstrVContent);
        }
        else
        {
            mstrVValue = mstrVContent;
         }
        strExpress =Calc.Replacestr(mstrexp,std::string("v"),mstrVValue);

    }else
    {
        //nothing to do
    }
    ///计算表达式
    if(Calc.expression(strExpress))
    {
        if(((0==Calc.mType)&&(Calc.mInt64Result>0))||((1==Calc.mType)&&(Calc.mdResult>0)))
        {
            mResult = true;
            mstrCalcResult="1";
            strOutText= "Judge v="+mstrVValue+"calc:"+mstrexp +"is true\n";
        }else{
           mResult = false;
           mstrCalcResult="0";
           strOutText= "Judge v="+mstrVValue+"calc:"+mstrexp +"is false\n";
        }
    }
    else
    {
         UT_SHOW("express Error(" + mstrexp+ ")");
    }
    return mResult;
}
std::string judgeStep::getVarValue(std::string str)
{
    std::string res = "";
    if(str == "Result")
    {
        res = mstrCalcResult;
    }
    return res;
}

bool judgeStep::getResult()
{
    return mResult;
}

std::string judgeStep::outText()
{
    return strOutText;
}

void judgeStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}
std::string judgeStep::getVarV(std::string use)
{
    Condition cond;
    return cond.getVarValue(mRunObj,use);
}
void judgeStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new judgeStep();
    }
}

    
    
