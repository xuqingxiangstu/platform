#include "keyParamShowStep.h"

#include "../../PfCommon/tools/ut_error.h"

keyParamShowStep::keyParamShowStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

keyParamShowStep::~keyParamShowStep()
{

}

void keyParamShowStep::init(const TiXmlElement *ele)
{

}

std::string keyParamShowStep::getClassName()
{
    return VAR_NAME(keyParamShowStep);
}

bool keyParamShowStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[keyParamShowStep]");
#endif

    return mResult;
}
std::string keyParamShowStep::getVarValue(std::string)
{
    return "";
}

bool keyParamShowStep::getResult()
{
    return mResult;
}

std::string keyParamShowStep::outText()
{
    return strOutText;
}

void keyParamShowStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void keyParamShowStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new keyParamShowStep();
    }
}

    
    