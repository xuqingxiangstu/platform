#include "judgeMultiStep.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../condition/condition.h"
#include "../../PfCommon/CalcFormula/calcformula.h"
judgeMultiStep::judgeMultiStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
    mvecVariate.clear();
    std::string strTemp[10]={"a","b","c","d","e","f","g","h","i","j"};
    for(int i=0;i<10;i++)
    {
      std::tuple<std::string,std::string,std::string> temp(strTemp[i],"","");
      mvecVariate.push_back(temp);
    }
}

judgeMultiStep::~judgeMultiStep()
{

}

void judgeMultiStep::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;
   ///获取输入的变量
   std::vector<std::tuple<std::string,std::string,std::string>>::iterator iter=mvecVariate.begin();
   for(;iter != mvecVariate.end(); iter++)
   {
       std::string Temp =std::get<0>(*iter);
       pTmpElem = xmlEle->FirstChildElement(Temp.c_str());
      if(pTmpElem)
      {
        const char *point = nullptr;
        point = pTmpElem->GetText();
        if(point)
            std::get<1>(*iter) = std::string(point);
      }
   }
   ///表达式
   pTmpElem = xmlEle->FirstChildElement("calc");
    if (pTmpElem)
    {
         const char *point = nullptr;
         point = pTmpElem->GetText();
         if(point)
            mstrexp =std::string(point);
     }
}

std::string judgeMultiStep::getClassName()
{
    return VAR_NAME(judgeMultiStep);
}

bool judgeMultiStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[judgeMultiStep]");
#endif
    std::string strExpress=mstrexp;
    Pf::PfCommon::CalcFormula Calc;
    std::vector<std::tuple<std::string,std::string,std::string>>::iterator iter=mvecVariate.begin();

    for(;iter != mvecVariate.end(); iter++)
    {

        if(mstrexp.find(std::get<0>(*iter))) //判断表达式中是否含变量名字
        {
            std::string strTemp=std::get<1>(*iter);
            //判断是否为引用
            if((strTemp.find("!") != std::string::npos)||(strTemp.find("@") != std::string::npos))
            {
                std::get<2>(*iter) = getVarV(strTemp);
            }
            else
            {
                std::get<2>(*iter) = strTemp;
             }
            strExpress =Calc.Replacestr(strExpress,std::get<0>(*iter),std::get<2>(*iter));

        }else
        {
            //nothing to do
        }
    }

    ///计算表达式
    if(Calc.expression(strExpress))
    {
        if(((0==Calc.mType)&&(Calc.mInt64Result>0))||((1==Calc.mType)&&(Calc.mdResult>0)))
        {
            mResult = true;
            mstrCalcResult="1";
            strOutText= "judgeMultiStep calc:"+mstrexp +"is true\n";
        }else{
           mResult = false;
           mstrCalcResult="0";
           strOutText= "judgeMultiStep calc:"+mstrexp +"is false\n";
        }
    }
    else
    {
         UT_SHOW("express Error(" + mstrexp+ ")");
    }
    return mResult;
}
std::string judgeMultiStep::getVarValue(std::string str)
{
    std::string res = "";
    if(str == "Result")
    {
        res = mstrCalcResult;
    }
    return res;
}
std::string judgeMultiStep::getVarV(std::string use)
{
    Condition cond;
    return cond.getVarValue(mRunObj,use);
}
bool judgeMultiStep::getResult()
{
    return mResult;
}

std::string judgeMultiStep::outText()
{
    return strOutText;
}

void judgeMultiStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void judgeMultiStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new judgeMultiStep();
    }
}

    
    
