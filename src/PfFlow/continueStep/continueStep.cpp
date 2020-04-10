#include "continueStep.h"

#include "../../PfCommon/tools/ut_error.h"

continueStep::continueStep():
mRunObj(nullptr),
mAdapter(nullptr),
strOutText(),
mResult(false)
{
}

continueStep::~continueStep()
{

}

void continueStep::init(const TiXmlElement *ele)
{

}

std::string continueStep::getClassName()
{
    return VAR_NAME(continueStep);
}

bool continueStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[continueStep]");
#endif
    mResult = true;

    return mResult;
}
std::string continueStep::getVarValue(std::string)
{
    return "";
}

bool continueStep::getResult()
{
    return mResult;
}

std::string continueStep::outText()
{
    return strOutText;
}

void continueStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void continueStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new continueStep();
    }
}

    
    
