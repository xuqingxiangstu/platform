#include "breakStep.h"

#include "../../PfCommon/tools/ut_error.h"

breakStep::breakStep():
mRunObj(nullptr),
mAdapter(nullptr),
strOutText(),
mResult(false)
{
    mCondition = std::make_shared<Condition>();
}

breakStep::~breakStep()
{

}

void breakStep::init(const TiXmlElement *xmlEle)
{
    std::string index = "";
    const char *point = xmlEle->Attribute(KeyAttr);
    if(point)
        index = std::string(point);

    try
    {
        mCondition->init(xmlEle);
    }
    catch(std::runtime_error err)
    {
        UT_THROW_EXCEPTION( "Index = " + index + " " + err.what());
    }
}

std::string breakStep::getClassName()
{
    return VAR_NAME(breakStep);
}

bool breakStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[breakStep]");
#endif
    mResult = mCondition->exec();
    return mResult;
}
std::string breakStep::getVarValue(std::string)
{
    return "";
}

bool breakStep::getResult()
{
    return mResult;
}

std::string breakStep::outText()
{
    return strOutText;
}

void breakStep::setAutoRunObj(runObj *obj)
{
    mCondition->setAutoRunObj(obj);
}

void breakStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new breakStep();
    }
}

    
    
