#include "getSwitchInStep.h"

#include "../../PfCommon/tools/ut_error.h"

getSwitchInStep::getSwitchInStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

getSwitchInStep::~getSwitchInStep()
{

}

void getSwitchInStep::init(const TiXmlElement *ele)
{

}

std::string getSwitchInStep::getClassName()
{
    return VAR_NAME(getSwitchInStep);
}

bool getSwitchInStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[getSwitchInStep]");
#endif

    return mResult;
}
std::string getSwitchInStep::getVarValue(std::string)
{
    return "";
}

bool getSwitchInStep::getResult()
{
    return mResult;
}

std::string getSwitchInStep::outText()
{
    return strOutText;
}

void getSwitchInStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void getSwitchInStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new getSwitchInStep();
    }
}

    
    