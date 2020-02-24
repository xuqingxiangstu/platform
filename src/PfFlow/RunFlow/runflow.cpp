#include "runflow.h"
#include "../../PfCommon/tools/ut_error.h"
#include <sstream>


RunFlow::RunFlow()
    :isLoad(false),
      mAutoRun(nullptr)
{
    mAutoRun = new autoRun();
}

RunFlow::~RunFlow()
{
    if(mAutoRun)
    {
        delete mAutoRun;
        mAutoRun = nullptr;
    }
}

void RunFlow::load(std::string strFilePath)
{
    std::stringstream strErr;
    TiXmlDocument mXmlDoc(strFilePath.c_str());

    if(!mXmlDoc.LoadFile())
    {
        strErr << "[ERROR](xml)Open File Error" << strFilePath << "\n";
        UT_THROW_EXCEPTION(strErr.str());
    }
    else
    {
        TiXmlElement *mRoot = mXmlDoc.RootElement();

        const char *point = mRoot->Attribute("version");
        if(point)
        {
            proVersion = std::string(point);
        }

        mAutoRun->init(mRoot);
    }

    isLoad = true;
}

void RunFlow::init(std::string strFilePath)
{
    if(isRunning())
       return ;

    if(isLoad)
    {
        ///先删除之前
        if(mAutoRun)
            mAutoRun->unInit();
    }

    load(strFilePath);
}

void RunFlow::unInit()
{

}

void RunFlow::setRunItem(std::string item)
{
    //autoRun::getInstance()->setRunItem(item);
}

void RunFlow::getRunItems(std::vector<std::string> &items)
{
    mAutoRun->getRunItems(items);
}

void RunFlow::runAllFlow()
{
    if(!isRunning())
        mAutoRun->runAllFlow();
}

bool RunFlow::isRunning()
{
    return mAutoRun->isRunning();
}

void RunFlow::setFlowType(std::string type)
{
    mAutoRun->setFlowType(type);
}

void RunFlow::setMsgObj(Pf::PfAdapter::Adapter *obj)
{
    mAutoRun->setMsgObj(obj);
}

void RunFlow::runFlow(std::string name)
{
    if(!isRunning())
        mAutoRun->runFlow(name);
}

//暂停测试
void RunFlow::suspendTest()
{
    mAutoRun->suspendTest();
}

//继续测试
void RunFlow::continueTest()
{
    mAutoRun->continueTest();
}

//停止所有测试
void RunFlow::exitAllTest()
{
    mAutoRun->exitAllTest();
}
