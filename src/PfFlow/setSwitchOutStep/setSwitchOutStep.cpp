#include "setSwitchOutStep.h"

#include "../../PfCommon/tools/ut_error.h"

setSwitchOutStep::setSwitchOutStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

setSwitchOutStep::~setSwitchOutStep()
{

}

void setSwitchOutStep::init(const TiXmlElement *ele)
{

}

std::string setSwitchOutStep::getClassName()
{
    return VAR_NAME(setSwitchOutStep);
}

bool setSwitchOutStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[setSwitchOutStep]");
#endif

    return mResult;
}
std::string setSwitchOutStep::getVarValue(std::string)
{
    return "";
}

bool setSwitchOutStep::getResult()
{
    return mResult;
}

std::string setSwitchOutStep::outText()
{
    return strOutText;
}

void setSwitchOutStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void setSwitchOutStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new setSwitchOutStep();
    }
}

    
    