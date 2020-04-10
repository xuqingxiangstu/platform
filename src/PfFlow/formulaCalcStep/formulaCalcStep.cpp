#include "formulaCalcStep.h"

#include "../../PfCommon/tools/ut_error.h"

formulaCalcStep::formulaCalcStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

formulaCalcStep::~formulaCalcStep()
{

}

void formulaCalcStep::init(const TiXmlElement *ele)
{

}

std::string formulaCalcStep::getClassName()
{
    return VAR_NAME(formulaCalcStep);
}

bool formulaCalcStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[formulaCalcStep]");
#endif

    return mResult;
}
std::string formulaCalcStep::getVarValue(std::string)
{
    return "";
}

bool formulaCalcStep::getResult()
{
    return mResult;
}

std::string formulaCalcStep::outText()
{
    return strOutText;
}

void formulaCalcStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void formulaCalcStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new formulaCalcStep();
    }
}

    
    