#include "sleepStep.h"
#include <thread>

#include "../../PfCommon/tools/ut_error.h"

sleepStep::sleepStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

sleepStep::~sleepStep()
{

}

void sleepStep::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;

    const char *point = nullptr;

    pTmpElem = xmlEle->FirstChildElement("timeout");
    if (pTmpElem)
    {
        point = pTmpElem->GetText();
        if(point)
            sscanf(point, "%d", &mTimeOut);
    }

    pTmpElem = xmlEle->FirstChildElement("Unit");
    if (pTmpElem)
    {
        point = pTmpElem->GetText();
        if(point)
            mUnit = std::string(point);
    }
}

std::string sleepStep::getClassName()
{
    return VAR_NAME(sleepStep);
}

bool sleepStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[sleepStep]");
#endif
    mResult = true;

    if(mUnit == "s")
        std::this_thread::sleep_for(std::chrono::seconds(mTimeOut)); //单位秒
    else
        std::this_thread::sleep_for(std::chrono::milliseconds(mTimeOut)); //单位是毫秒

    return mResult;
}
std::string sleepStep::getVarValue(std::string var)
{
    std::string res = "";

    if(var == "timeout")
    {
        res = std::to_string(mTimeOut);
    }

    return res;
}

bool sleepStep::getResult()
{
    return mResult;
}

std::string sleepStep::outText()
{
    return strOutText;
}

void sleepStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void sleepStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new sleepStep();
    }
}

    
    
