#include "temperatureStep.h"

#include "../../PfCommon/tools/ut_error.h"

temperatureStep::temperatureStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

temperatureStep::~temperatureStep()
{

}

void temperatureStep::init(const TiXmlElement *ele)
{

}

std::string temperatureStep::getClassName()
{
    return VAR_NAME(temperatureStep);
}

bool temperatureStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[temperatureStep]");
#endif

    return mResult;
}
std::string temperatureStep::getVarValue(std::string)
{
    return "";
}

bool temperatureStep::getResult()
{
    return mResult;
}

std::string temperatureStep::outText()
{
    return strOutText;
}

void temperatureStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void temperatureStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new temperatureStep();
    }
}

    
    