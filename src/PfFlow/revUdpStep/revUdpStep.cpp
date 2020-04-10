#include "revUdpStep.h"
#include <regex>
#include "../../PfCommon/tools/ut_error.h"
#include "../../PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../../PfCommon/jsoncpp/json.h"
#include <sstream>
#include <iostream>
using namespace Pf;
revUdpStep::revUdpStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    strJson(),
    mResult(false)
{
}

revUdpStep::~revUdpStep()
{

}

void revUdpStep::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;

    const char *point = nullptr;

    pTmpElem = xmlEle->FirstChildElement("RevAdpId");
    if (pTmpElem)
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
    pTmpElem = xmlEle->FirstChildElement("UploadDataAdpId");
    if (pTmpElem)
    {
        point = pTmpElem->GetText();

        if(point)
            strUploadDataId = std::string(point);
    }
    pTmpElem = xmlEle->FirstChildElement("QuoteVar");
    if (pTmpElem)
    {
        point = pTmpElem->GetText();

        if(point)
        {
            std::string strTemp = std::string(point);
            vecQuoteVar.clear();
            int off=0;
            int npos=0;
            for(unsigned int i=0;i<strTemp.size();i++)
            {
                if(strTemp[i]==';')
                {
                    npos=i-off;
                    vecQuoteVar.push_back(strTemp.substr(off,npos));
                    off=i+1;//跳过
                }
            }
            npos=strTemp.size()-off;
            vecQuoteVar.push_back(strTemp.substr(off,npos));
        }
    }

}

std::string revUdpStep::getClassName()
{
    return VAR_NAME(revUdpStep);
}

bool revUdpStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[revUdpStep]");
#endif
    strJson.clear();
    const int BUF_SIZE = 10240;
    char buf[BUF_SIZE] = {0};
    int rcvSize = 0;
    Pf::PfAdapter::Adapter *RcvObj;
    Pf::PfAdapter::Adapter *UpDataObj;
    mRevResult = false;
    if(!(mAdapter->getAdapter(strDevId, &RcvObj)))
    {
        UT_SHOW("get Adapter(" + strDevId + ") faild!");
        mResult = false;
        return mResult;
    }
    if(!(mAdapter->getAdapter(strUploadDataId, &UpDataObj)))
    {
        UT_SHOW("get Adapter(" + strDevId + ") faild!");
        mResult = false;
        return mResult;
    }

    //TODO:fjt
    if(RcvObj->receiveMsg(buf,rcvSize, BUF_SIZE))
    {
       try
       {
            auto obj = mIcdAdapter->getFrameObj(strProtocolId);
            if(obj != nullptr)
            {
                strJson =obj->parse((unsigned char *)buf,rcvSize);
                UpDataObj->sendMsg(strJson.c_str(),strJson.size());
            }
            mRevResult = true;
        }
        catch(std::runtime_error err)
        {
            UT_SHOW("get IcdAdapter(" + std::string(err.what()) + ") faild!");
        }
    }
    else
    {
        //nothing to do
    }
    return mResult;
}
std::string revUdpStep::getJsonParamValue(std::string strParamName)
{
    std::string strResult="";
    Json::Value root;
    Json::Reader reader;
    if(reader.parse(strJson,root))
    {
        Json::Value head = root["head"];
        Json::Value infoword = root["infoWord"][0];
        Json::Value JsonRes;
        if(head.isMember(strParamName))
        {
            JsonRes =head[strParamName];
        }else if(infoword.isMember(strParamName))
        {
            JsonRes =infoword[strParamName];
        }
        else
        {
            UT_SHOW("get JsonParamValue(" + strParamName + ") faild!");
            return "";
        }
        if(JsonRes.isIntegral())
        {
            strResult =std::to_string(JsonRes.asInt());
        }
        else if(JsonRes.isDouble())
        {
            strResult =std::to_string(JsonRes.asDouble());
        }else if(JsonRes.isBool())
        {
            strResult =std::to_string(JsonRes.asBool());
        }
    }
    return strResult;
}
std::string revUdpStep::getVarValue(std::string src)
{
    std::string res = "";
    ///Value[0,1]
    std::regex reg("^QuoteVar\\[(\\d+)\\,(\\d+)\\]", std::regex::icase);
    std::smatch matchRes;
    if(std::regex_match(src, matchRes, reg))
    {
       int index=1;
       std::string strIndex=matchRes[index++];
       if(std::stoi(strIndex)<vecQuoteVar.size())
       {
           res=getJsonParamValue(vecQuoteVar.at(std::stoi(strIndex)));
       }else
       {
           UT_SHOW("get Quote surpass max(" + strIndex + ") is too big!");
       }
    }
    else if(src=="Result")
    {
        res = std::to_string(mRevResult);
    }
    return res;
}

bool revUdpStep::getResult()
{
    return mResult;
}

std::string revUdpStep::outText()
{
    return strOutText;
}

void revUdpStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void revUdpStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new revUdpStep();
    }
}

    
    
