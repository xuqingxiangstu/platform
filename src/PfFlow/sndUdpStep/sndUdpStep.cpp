#include "sndUdpStep.h"

#include "../../PfCommon/tools/ut_error.h"

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

void sndUdpStep::init(const TiXmlElement *ele)
{

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

    
    