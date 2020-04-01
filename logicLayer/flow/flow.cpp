#include "flow.h"
#include "../src/PfCommon/tools/ut_error.h"

#include "condition.h"
#include <thread>

#include <QDateTime>
#include <QDebug>
#include <thread>

flowManager::flowManager():
    mIsRunning(false),
    mEqSystemUuid("")
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

        tmpJs["uuid"] = std::get<Uuid_Index>(flow);
        tmpJs["describe"] = std::get<Des_Index>(flow);
        tmpJs["sub_flow"] = std::get<Flow_Index>(flow)->getRunItems();

        flowJs.append(tmpJs);
    }    

    return flowJs;
}

void flowManager::exeOver()
{
    //运行结束

    Json::Value js;
    js["msgType"] = "testOver";

    Json::Value tmp;
    tmp["record_uuid"] = mRecordUuid;

    tmp["result"] = true;

    js["msg"] = tmp;

    if(mUiAdapter)
        mUiAdapter->sendMsg(js.toStyledString().c_str(), js.toStyledString().size());

    mIsRunning = false;
}

void flowManager::exe(std::string flowUuid, std::string subFlowUuid)
{
    if(!mIsRunning)
    {
        if(flowUuid == "")//如果不指定则全部运行，否则单个运行
        {
            for(auto obj : mFlowsObj)
            {
                std::thread thread([=](){

                #ifdef DEBUG_PRINT
                        UT_SHOW("-------------" + std::get<0>(flow) + "-------------");
                #endif
                        std::get<Flow_Index>(obj)->exe(this, subFlowUuid);

                });
                thread.detach();

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
                std::thread thread([=]{
                   std::get<Flow_Index>(*findItor)->exe(this, subFlowUuid);
                });

                thread.detach();

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

flow::flow(TiXmlElement *xmlEle)
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

void flow::exe(flowManager *manager, std::string subFlowUuid)
{   
    mFlowManagerObj = manager;

    if(subFlowUuid == "") //全部执行
    {
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

        std::vector<std::thread> threads;

        for(auto obj : threadExes)
        {
            threads.push_back(std::thread([=](){
                                  obj->exe();
                              }));
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
            thread.join();
        }

        mFlowManagerObj->exeOver();
    }
    else//执行某一个，此时不需要开启线程，上级已经开线程
    {
        auto findItor = std::find_if(mSubFlowObjs.begin(), mSubFlowObjs.end(), [=](const std::tuple<std::string, std::shared_ptr<subFlow>> &v){
           return std::get<Uuid_Index>(v) == subFlowUuid;
        });

        if(findItor != mSubFlowObjs.end())
        {
            std::get<SubFlow_Index>(*findItor)->exe();
        }
        else
        {
            throw std::runtime_error("[ERROR] not exist sub flow (" + subFlowUuid + ")");
        }
    }
}
