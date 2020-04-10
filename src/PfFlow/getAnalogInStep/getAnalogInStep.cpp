#include "getAnalogInStep.h"

#include "../../PfCommon/tools/ut_error.h"

//#include "../../PfAdapter/analogIn/AnalogInAdapter.h"

getAnalogInStep::getAnalogInStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

getAnalogInStep::~getAnalogInStep()
{

}

void getAnalogInStep::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;

    const char *point = nullptr;

    pTmpElem = xmlEle->FirstChildElement("DevId");
    if (pTmpElem)
    {
        point = pTmpElem->GetText();

        if(point)
            strDevId = std::string(point);
    }

    pTmpElem = xmlEle->FirstChildElement("Channel");
    if (pTmpElem)
    {
        point = pTmpElem->GetText();

        if(point)
            sscanf(std::string(point).c_str(), "%d", &mChannel);
    }

    if( (strDevId == "") || (mChannel == 0))
    {
        UT_THROW_EXCEPTION("devid channel faild => " + strDevId + std::to_string(mChannel));
    }
}

std::string getAnalogInStep::getClassName()
{
    return VAR_NAME(getAnalogInStep);
}

bool getAnalogInStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[getAnalogInStep]");
#endif

    Pf::PfAdapter::Adapter *obj;

    if(mAdapter->getAdapter(strDevId, &obj))
    {
        double value = 0;
        //TODO:xqx
#if 0
        if((dynamic_cast<analogInOpt*>(obj))->readValue(mChannel, value))
        {
            strValue = std::to_string(value);

            mResult = true;
        }
        else
        {
#ifndef QT_NO_DEBUG
            UT_SHOW("readValue(" + strDevId + "," + std::to_string(mChannel) + ") faild!");
#endif
        }
#endif
    }
    else
    {
#ifndef QT_NO_DEBUG
        UT_SHOW("get devid(" + strDevId + ") faild!");
#endif
        mResult = false;
    }

    return mResult;
}
std::string getAnalogInStep::getVarValue(std::string var)
{
    std::string res = "";

    if(var == "Value")
    {
        res = strValue;
    }

    return res;
}

bool getAnalogInStep::getResult()
{
    return mResult;
}

std::string getAnalogInStep::outText()
{
    return strOutText;
}

void getAnalogInStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void getAnalogInStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new getAnalogInStep();
    }
}

    
    
