#include "elseStep.h"

#include "../../PfCommon/tools/ut_error.h"

elseStep::elseStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

elseStep::~elseStep()
{

}

void elseStep::init(const TiXmlElement *ele)
{

}

std::string elseStep::getClassName()
{
    return VAR_NAME(elseStep);
}

bool elseStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[elseStep]");
#endif
    mResult = true;
    return mResult;
}
std::string elseStep::getVarValue(std::string)
{
    return "";
}

bool elseStep::getResult()
{
    return mResult;
}

std::string elseStep::outText()
{
    return strOutText;
}

void elseStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void elseStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new elseStep();
    }
}

    
    
