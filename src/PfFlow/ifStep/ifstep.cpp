#include "ifstep.h"

#include "../../PfCommon/tools/ut_error.h"

ifStep::ifStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(""),
    mResult(false),
    mCondition(nullptr)
{
    mCondition = std::make_shared<Condition>();
}

ifStep::~ifStep()
{

}

void ifStep::init(const TiXmlElement *xmlEle)
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


std::string ifStep::getClassName()
{
    return VAR_NAME(ifStep);
}

bool ifStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[ifStep]");
#endif    
    mResult = mCondition->exec();
    return mResult;
}
std::string ifStep::getVarValue(std::string)
{
    return "";
}

bool ifStep::getResult()
{
    return mResult;
}

std::string ifStep::outText()
{
    return strOutText;
}

void ifStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
    mCondition->setAutoRunObj(obj);
}

void ifStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new ifStep();
    }
}
