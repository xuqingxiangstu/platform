#include "flow.h"

#include "../../PfCommon/tools/ut_error.h"
#include <sstream>


flow::flow()
    :isLoad(false),
      mAutoRun(nullptr)
{
    mAutoRun = new autoRun();
}

flow::~flow()
{
    if(mAutoRun)
    {
        delete mAutoRun;
        mAutoRun = nullptr;
    }
}

void flow::load(std::string strFilePath)
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

void flow::init(std::string strFilePath)
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

void flow::unInit()
{

}

void flow::setRunItem(std::string item)
{
    //autoRun::getInstance()->setRunItem(item);
}

void flow::getRunItems(std::vector<std::string> &items)
{
    mAutoRun->getRunItems(items);
}

void flow::runAllFlow()
{
    if(!isRunning())
        mAutoRun->runAllFlow();
}

bool flow::isRunning()
{
    return mAutoRun->isRunning();
}

void flow::setFlowType(std::string type)
{
    mAutoRun->setFlowType(type);
}

void flow::setMsgObj(Pf::PfAdapter::Adapter *obj)
{
    mAutoRun->setMsgObj(obj);
}

void flow::runFlow(std::string name)
{
    if(!isRunning())
        mAutoRun->runFlow(name);
}

//暂停测试
void flow::suspendTest()
{
    mAutoRun->suspendTest();
}

//继续测试
void flow::continueTest()
{
    mAutoRun->continueTest();
}

//停止所有测试
void flow::exitAllTest() 
{
    mAutoRun->exitAllTest();
}
