#include "flow.h"
#include "../src/PfCommon/tools/ut_error.h"

#include "condition.h"
#include <thread>

#include <QDateTime>
#include <QDebug>
#include <thread>
#include <QThread>
#include <QObject>

flowManager::flowManager(QObject *parent):
    QObject(parent),
    mIsRunning(false),
    mEqSystemUuid(""),
    mDstSystemUuid(""),
    mEqProtocol("")
{
}

flowManager::~flowManager()
{
    exit();
}

void flowManager::exit()
{
    if(mIsRunning)
    {
        for(auto flow : mFlowsObj)
        {
            std::get<Flow_Index>(flow)->exit();
        }

        mIsRunning = false;
    }
}

void flowManager::setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj)
{
    for(auto flow : mFlowsObj)
    {
        std::get<Flow_Index>(flow)->setAdapterManager(adapterManagerObj);
    }
    //获取发送UI句柄
    adapterManagerObj->getAdapter("UI", &mUiAdapter);
}

void flowManager::setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter)
{
    for(auto flow : mFlowsObj)
    {
        std::get<Flow_Index>(flow)->setIcdAdapter(icdAdapter);
    }
}

void flowManager::setRecordUuid(std::string uuid)
{
    mRecordUuid = uuid;

    for(auto flow : mFlowsObj)
    {
        std::get<Flow_Index>(flow)->setRecordUuid(uuid);
    }
}

Json::Value flowManager::getRunItems()
{
    Json::Value flowJs;

    for(auto flow : mFlowsObj)
    {
        Json::Value tmpJs;

        tmpJs["flow_uuid"] = std::get<Uuid_Index>(flow);
        tmpJs["describe"] = std::get<Des_Index>(flow);
        tmpJs["sub_flow"] = std::get<Flow_Index>(flow)->getRunItems();

        flowJs.append(tmpJs);
    }    

    return flowJs;
}

void flowManager::exeOver(bool status, std::string info)
{
    //运行结束

    Json::Value js;
    js["msgType"] = "testOver";

    Json::Value tmp;
    tmp["record_uuid"] = mRecordUuid;
    tmp["result"] = status;

    tmp["info"] = info;

    js["msg"] = tmp;

    if(mUiAdapter)
        mUiAdapter->sendMsg(js.toStyledString().c_str(), js.toStyledString().size());

    mIsRunning = false;   

#if !USE_STD_THRED
    //释放线程句柄
    for(auto itor = mExeThreads.begin(); itor != mExeThreads.end(); )
    {
        (*itor).reset();

        mExeThreads.erase(itor);
    }
#endif
}

void flowManager::enforceExe(std::string flowUuid, std::string subFlowUuid)
{
    if(mIsRunning)
    {
        //查找uuid相关流程
        auto findItor = std::find_if(mFlowsObj.begin(), mFlowsObj.end(), [=](const std::tuple<std::string, std::string, std::shared_ptr<flow>> &v){
            return std::get<Uuid_Index>(v) == flowUuid;
        });

        if(findItor != mFlowsObj.end())
        {
           std::get<Flow_Index>(*findItor)->enforceExe(subFlowUuid);
        }
    }
}

void flowManager::exe(std::string flowUuid, std::string subFlowUuid)
{
    if(!mIsRunning)
    {
        if(flowUuid == "")//如果不指定则全部运行，否则单个运行
        {
            for(auto obj : mFlowsObj)
            {

#if USE_STD_THRED
                std::thread thread([=](){
#else
                std::shared_ptr<QThread> thread = std::make_shared<QThread>();

                QObject::connect(thread.get(), &QThread::started, [=](){
#endif

                #ifdef DEBUG_PRINT
                        UT_SHOW("-------------" + std::get<0>(flow) + "-------------");
                #endif
                        std::get<Flow_Index>(obj)->exe(this, subFlowUuid);

                });
#if USE_STD_THRED
                thread.detach();
#else
                thread->start();

                mExeThreads.emplace_back(thread);
#endif

                mIsRunning = true;
            }            
        }
        else
        {
            //查找uuid相关流程
            auto findItor = std::find_if(mFlowsObj.begin(), mFlowsObj.end(), [=](const std::tuple<std::string, std::string, std::shared_ptr<flow>> &v){
                return std::get<Uuid_Index>(v) == flowUuid;
            });

            if(findItor != mFlowsObj.end())
            {
#if USE_STD_THRED
                std::thread thread([=](){
#else
                std::shared_ptr<QThread> thread = std::make_shared<QThread>();

                QObject::connect(thread.get(), &QThread::started, [=](){
#endif

                   std::get<Flow_Index>(*findItor)->exe(this, subFlowUuid);

                });

#if USE_STD_THRED
                thread.detach();
#else
                thread->start();

                mExeThreads.emplace_back(thread);
#endif

                mIsRunning = true;
            }
            else
            {
                throw std::runtime_error("[ERROR] not exist flow (" + flowUuid + ")");
            }
        }
    }
}

void flowManager::init(std::string strFilePath)
{
    std::stringstream strErr;
    std::stringstream allErr;
    TiXmlDocument *mXmlDoc = new TiXmlDocument(strFilePath.c_str());

    if(!mXmlDoc->LoadFile())
    {
        strErr << "[ERROR](xml)Open File Error" << strFilePath << "\n";
        UT_THROW_EXCEPTION(strErr.str());
    }
    else
    {
        TiXmlElement *mRoot = mXmlDoc->RootElement();

        TiXmlElement *mEqEle = mRoot->FirstChildElement("equivalent");
        if(mEqEle)
        {
            TiXmlElement *uuidEle = mEqEle->FirstChildElement("system_uuid");
            if(uuidEle)
            {
                mEqSystemUuid = uuidEle->GetText();
            }

            TiXmlElement *dstUuidEle = mEqEle->FirstChildElement("dest_system_uuid");
            if(dstUuidEle)
            {
                mDstSystemUuid = dstUuidEle->GetText();
            }

            TiXmlElement *ptlEle = mEqEle->FirstChildElement("protocol");
            if(ptlEle)
            {
                mEqProtocol = ptlEle->GetText();
            }

            TiXmlElement *askEle = mEqEle->FirstChildElement("askflag");
            if(askEle)
            {
                mAskFlag = askEle->GetText();
            }
        }

        TiXmlElement *mChildEle = mRoot->FirstChildElement("flow");

        for (mChildEle; mChildEle; mChildEle = mChildEle->NextSiblingElement("flow"))
        {
            const char *point = nullptr;
            std::string des = "";
            std::string uuid = "";

            point = mChildEle->Attribute("uuid");
            if(point)
            {
                uuid = std::string(point);
            }

            point = mChildEle->Attribute("describe");
            if(point)
            {
                des = std::string(point);
            }

            std::shared_ptr<flow> obj = std::make_shared<flow>(mChildEle);
            obj->setFlowUuid(uuid);

            mFlowsObj.emplace_back(std::make_tuple(uuid, des, obj));
        }
    }
}

flow::flow(TiXmlElement *xmlEle, QObject *parent):
    QObject(parent)
{
    TiXmlElement *mChildEle = xmlEle->FirstChildElement("subFlow");

    for (mChildEle; mChildEle; mChildEle = mChildEle->NextSiblingElement("subFlow"))
    {
        std::string uuid = mChildEle->Attribute("uuid");

        std::shared_ptr<subFlow> obj = std::make_shared<subFlow>(mChildEle);

        mUuid = uuid;

        obj->setFlowUuid(mUuid);

        mSubFlowObjs.push_back(std::make_tuple(uuid, obj));
    }

    isStop = false;
}

 void flow::setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj)
 {
     for(auto obj : mSubFlowObjs)
     {
         std::get<SubFlow_Index>(obj)->setAdapterManager(adapterManagerObj);
     }

     //获取发送UI句柄
     adapterManagerObj->getAdapter("UI", &mUiAdapter);
 }

 void flow::setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter)
 {
     for(auto obj : mSubFlowObjs)
     {
         std::get<SubFlow_Index>(obj)->setIcdAdapter(icdAdapter);
     }
 }

 void flow::setRecordUuid(std::string uuid)
 {
     mRecordUuid = uuid;

     for(auto obj : mSubFlowObjs)
     {
         std::get<SubFlow_Index>(obj)->setRecordUuid(uuid);
     }
 }

void flow::exit()
{
    for(auto obj : mSubFlowObjs)
    {
        std::get<SubFlow_Index>(obj)->exit();
    }

    isStop = true;
}

Json::Value flow::getRunItems()
{
    Json::Value subFlowJs;

    for(auto obj : mSubFlowObjs)
    {
        subFlowJs.append(std::get<SubFlow_Index>(obj)->getRunItems());
    }

    return subFlowJs;
}

void flow::restoreInitStateCmd()
{
    Json::Value js;
    js["msgType"] = "restore_initial_state";

    Json::Value tmp;
    tmp["record_uuid"] = mRecordUuid;
    tmp["flow_uuid"] = mUuid;

    js["msg"] = tmp;

    if(mUiAdapter)
        mUiAdapter->sendMsg(js.toStyledString().c_str(), js.toStyledString().size());
}

void flow::enforceExe(std::string subFlowUuid)
{
    auto findItor = std::find_if(mSubFlowObjs.begin(), mSubFlowObjs.end(), [=](const std::tuple<std::string, std::shared_ptr<subFlow>> &v){
       return std::get<Uuid_Index>(v) == subFlowUuid;
    });

    bool res = false;
    std::string errInfo = "";
    if(findItor != mSubFlowObjs.end())
    {
       std::get<SubFlow_Index>(*findItor)->enforceExe();
       res = true;
    }
}

void flow::exe(flowManager *manager, std::string subFlowUuid)
{   
    isStop = false;
    mFlowManagerObj = manager;

    if(subFlowUuid == "") //全部执行
    {
        //modify xqx 2020-5-26 13:36:24 增加老化测试，执行完成后再次执行，直到人点击停止测试
#ifdef LH_TEST
        while(!isStop)
#endif
        {
        //end
            std::vector<std::shared_ptr<subFlow>> threadExes;

            std::vector<std::shared_ptr<subFlow>> orderExes;

            //执行前先检查哪些周期性执行，周期性执行则线程执行
            for(auto obj : mSubFlowObjs)
            {
                if(timing::CYCLE == std::get<SubFlow_Index>(obj)->getTimingType())
                {
                    threadExes.push_back(std::get<SubFlow_Index>(obj));
                }
                else
                {
                    orderExes.push_back(std::get<SubFlow_Index>(obj));
                }
            }

            //周期性执行
#if USE_STD_THRED
            std::vector<std::thread> threads;
#else
            std::vector<QThread*> threads;
#endif

            for(auto obj : threadExes)
            {
#if USE_STD_THRED
                threads.push_back(std::thread([=](){

                                      obj->exe();

                                  }));
#else
                QThread *thread = new QThread();

                QObject::connect(thread, &QThread::started, [=](){
                    obj->exe();
                });

                thread->start();

                threads.emplace_back(thread);
#endif

            }

            //顺序执行
            for(auto obj : orderExes)
            {
                if(isStop)
                    break;

                obj->exe();
            }

            //等待线程执行结束/手动退出
            for(auto& thread : threads)
            {
#if USE_STD_THRED
                thread.join();
#else
                thread->wait();

                delete thread;
                thread = nullptr;
#endif
            }


#ifdef LH_TEST
            if(isStop)
                break;

            //modify xqx 2020-5-26 13:49:19 本次结束后向界面发送初恢复始化状态
            restoreInitStateCmd();
            //end
#endif
        }

        mFlowManagerObj->exeOver(true, "");

        isStop = false;
    }
    else//执行某一个，此时不需要开启线程，上级已经开线程
    {
        auto findItor = std::find_if(mSubFlowObjs.begin(), mSubFlowObjs.end(), [=](const std::tuple<std::string, std::shared_ptr<subFlow>> &v){
           return std::get<Uuid_Index>(v) == subFlowUuid;
        });

        bool res = false;
        std::string errInfo = "";
        if(findItor != mSubFlowObjs.end())
        {
           std::get<SubFlow_Index>(*findItor)->exe();
           res = true;
        }
        else
        {
            errInfo =  "[ERROR] not exist sub flow (" + subFlowUuid + ")";
        }

        mFlowManagerObj->exeOver(res, errInfo);
    }
}
