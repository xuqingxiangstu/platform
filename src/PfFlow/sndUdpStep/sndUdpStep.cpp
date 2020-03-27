#include "sndUdpStep.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../../PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include <QDebug>
using namespace Pf;
sndUdpStep::sndUdpStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

sndUdpStep::~sndUdpStep()
{

}

void sndUdpStep::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;

    const char *point = nullptr;

    pTmpElem = xmlEle->FirstChildElement("SndAdpId");
    if(pTmpElem)
    {
        point = pTmpElem->GetText();
        if(point)
           strDevId = std::string(point);
    }
    pTmpElem = xmlEle->FirstChildElement("ProtocolId");
     if (pTmpElem)
     {
         point = pTmpElem->GetText();

         if(point)
             strProtocolId = std::string(point);
     }
    pTmpElem = xmlEle->FirstChildElement("headJson");
    if(pTmpElem)
    {
        point = pTmpElem->GetText();
        if(point)
           strJson = "{\"head\":\n{\n"+std::string(point)+"},\n\"infoWord\":\n[\n";
    }
    pTmpElem = xmlEle->FirstChildElement("infoWord1");
    {
        if(pTmpElem)
        {
            point = pTmpElem->GetText();
            if(point)
               strJson +="{\n" + std::string(point) + "\n}";
        }
    }
    pTmpElem = xmlEle->FirstChildElement("infoWord2");
    {
        if(pTmpElem)
        {
            point = pTmpElem->GetText();
            if(point&&(std::string(point)!=""))
               strJson +=",{\n" + std::string(point) + "\n}";
        }
    }
    pTmpElem = xmlEle->FirstChildElement("infoWord3");
    {
        if(pTmpElem)
        {
            point = pTmpElem->GetText();
            if(point&&(std::string(point)!=""))
               strJson +=",{\n" + std::string(point) + "\n}";
        }
    }
    pTmpElem = xmlEle->FirstChildElement("infoWord4");
    {
        if(pTmpElem)
        {
            point = pTmpElem->GetText();
            if(point&&(std::string(point)!=""))
               strJson +=",{\n" + std::string(point) + "\n}";
        }
    }
    pTmpElem = xmlEle->FirstChildElement("infoWord5");
    {
        if(pTmpElem)
        {
            point = pTmpElem->GetText();
            if(point)
               strJson +=",{\n" + std::string(point) + "\n}";
        }
    }
    strJson += "\n]\n}";
    strJson=replaceAll(strJson,";",",");
}
std::string&  sndUdpStep::replaceAll(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)
{
    while(true)   {
        std::string::size_type   pos(0);
        if((pos=str.find(old_value))!=std::string::npos)
            str.replace(pos,old_value.length(),new_value);
        else   break;
    }
    return   str;
}
std::string sndUdpStep::getClassName()
{
    return VAR_NAME(sndUdpStep);
}

bool sndUdpStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[sndUdpStep]");
#endif
    PfAdapter::Adapter *SndObj;
    PfIcdWorkBench::byteArray sendMsg;
    if(!(mAdapter->getAdapter(strDevId,&SndObj)))
    {
        UT_SHOW("get SndAdapter(" + strDevId + ") faild!");
        mResult = false;
        return mResult;
    }
     qDebug() << strJson.c_str();
    try{
        auto obj = mIcdAdapter->getFrameObj(strProtocolId);

        obj->simulation(sendMsg, strJson);
        SndObj->sendMsg((const char*)sendMsg.data(),sendMsg.size());
    }
    catch(std::runtime_error err)
    {
         UT_SHOW("get IcdAdapter(" + std::string(err.what()) + ") faild!");
    }
    return mResult;
}
std::string sndUdpStep::getVarValue(std::string)
{
    return "";
}

bool sndUdpStep::getResult()
{
    return mResult;
}

std::string sndUdpStep::outText()
{
    return strOutText;
}

void sndUdpStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void sndUdpStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new sndUdpStep();
    }
}

    
    
