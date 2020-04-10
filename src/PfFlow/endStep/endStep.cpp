#include "endStep.h"

#include "../../PfCommon/tools/ut_error.h"

endStep::endStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

endStep::~endStep()
{

}

void endStep::init(const TiXmlElement *ele)
{

}

std::string endStep::getClassName()
{
    return VAR_NAME(endStep);
}

bool endStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[endStep]");
#endif

    return mResult;
}
std::string endStep::getVarValue(std::string)
{
    return "";
}

bool endStep::getResult()
{
    return mResult;
}

std::string endStep::outText()
{
    return strOutText;
}

void endStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void endStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new endStep();
    }
}

    
    
