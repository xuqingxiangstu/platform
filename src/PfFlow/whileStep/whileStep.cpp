#include "whileStep.h"

#include "../../PfCommon/tools/ut_error.h"

whileStep::whileStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
    mCondition = std::make_shared<Condition>();
}

whileStep::~whileStep()
{

}

void whileStep::init(const TiXmlElement *xmlEle)
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

std::string whileStep::getClassName()
{
    return VAR_NAME(whileStep);
}

bool whileStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[whileStep]");
#endif
    mResult = mCondition->exec();

    return mResult;
}
std::string whileStep::getVarValue(std::string)
{
    return "";
}

bool whileStep::getResult()
{
    return mResult;
}

std::string whileStep::outText()
{
    return strOutText;
}

void whileStep::setAutoRunObj(runObj *obj)
{
    mCondition->setAutoRunObj(obj);
}

void whileStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new whileStep();
    }
}

    
    
