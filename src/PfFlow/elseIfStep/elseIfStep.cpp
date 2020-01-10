#include "elseIfStep.h"

#include "../../PfCommon/tools/ut_error.h"

elseIfStep::elseIfStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
    mCondition = std::make_shared<Condition>();
}

elseIfStep::~elseIfStep()
{

}

void elseIfStep::init(const TiXmlElement *xmlEle)
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

std::string elseIfStep::getClassName()
{
    return VAR_NAME(elseIfStep);
}

bool elseIfStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[elseIfStep]");
#endif

    mResult = mCondition->exec();

    return mResult;
}
std::string elseIfStep::getVarValue(std::string)
{
    return "";
}

bool elseIfStep::getResult()
{
    return mResult;
}

std::string elseIfStep::outText()
{
    return strOutText;
}

void elseIfStep::setAutoRunObj(runObj *obj)
{
    mCondition->setAutoRunObj(obj);
}

void elseIfStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new elseIfStep();
    }
}

    
    
