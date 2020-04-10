#include "setAnalogOutStep.h"

#include "../../PfCommon/tools/ut_error.h"

setAnalogOutStep::setAnalogOutStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

setAnalogOutStep::~setAnalogOutStep()
{

}

void setAnalogOutStep::init(const TiXmlElement *ele)
{

}

std::string setAnalogOutStep::getClassName()
{
    return VAR_NAME(setAnalogOutStep);
}

bool setAnalogOutStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[setAnalogOutStep]");
#endif

    return mResult;
}
std::string setAnalogOutStep::getVarValue(std::string)
{
    return "";
}

bool setAnalogOutStep::getResult()
{
    return mResult;
}

std::string setAnalogOutStep::outText()
{
    return strOutText;
}

void setAnalogOutStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void setAnalogOutStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new setAnalogOutStep();
    }
}

    
    