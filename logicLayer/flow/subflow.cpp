#include "subflow.h"


#include "../src/PfCommon/tools/ut_error.h"
#include "condition.h"
#include "action.h"
#include <thread>
#include <chrono>
#include <sstream>

#include <QDateTime>

subFlow::subFlow(TiXmlElement *element)
    :mStartCond(nullptr),
    mStopCond(nullptr),
    mAction(nullptr),
    mTiming(nullptr),
    isStop(false),
    mUuid(""),
    mDescribe(""),
    mUiAdapter(nullptr)
{
    init(element);
}

subFlow::~subFlow()
{
    isStop = true;
}

void subFlow::init(TiXmlElement *element)
{
    std::ostringstream strErr;
    const char *point = nullptr;

    point = element->Attribute("uuid");
    if(point)
        mUuid = std::string(point);

    point = element->Attribute("describe");
    if(point)
        mDescribe = std::string(point);

    TiXmlElement *ele = nullptr;

    ele = element->FirstChildElement("timing");
    if(ele == nullptr)
    {
        UT_THROW_EXCEPTION("get element (timing) faild!");
    }

    mTiming = std::make_shared<timing>();
    mTiming->init(ele);

    ele = element->FirstChildElement("startCondition");
    if(ele)
    {
        mStartCond = std::make_shared<startCondition>();
        mStartCond->init(ele);
    }


    ele = element->FirstChildElement("stopCondition");
    if(ele)
    {
        mStopCond = std::make_shared<stopCondition>();
        mStopCond->init(ele);
    }

    ele = element->FirstChildElement("action");
    if(ele == nullptr)
    {
        UT_THROW_EXCEPTION("get element (action) faild!");
    }

    mAction = std::make_shared<action>();
    mAction->init(ele);
}

timing::timingType subFlow::getTimingType()
{
    return mTiming->getType();
}

Json::Value subFlow::getRunItems()
{
    Json::Value subFlowJs = mAction->getRunItems();;

    if(subFlowJs["info"].size() > 0)
    {
        subFlowJs["table"] = subFlowJs["info"][0]["table"].asString();
    }

    subFlowJs["uuid"] = mUuid;
    subFlowJs["describe"] = mDescribe;
    subFlowJs["timing"] = mTiming->getTimingStr();

    if(mStartCond)
    {
        Json::Value tmp = mStartCond->getRunItems();
        if(tmp["trigger"])
            subFlowJs["trigger"] = tmp["trigger"].asString();
    }

    return subFlowJs;
}

void subFlow::setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj)
{
    if(mAction.get() != nullptr)
        mAction->setAdapterManager(adapterManagerObj);

    //获取发送UI句柄
    adapterManagerObj->getAdapter("UI", &mUiAdapter);
}

void subFlow::setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter)
{
    if(mAction.get() != nullptr)
        mAction->setIcdAdapter(icdAdapter);
}

void subFlow::setRecordUuid(std::string uuid)
{
    mRecordUuid = uuid;
}

void subFlow::exe()
{  
    //如果有启动条件则判断
    if(mStartCond)
    {
        while( (!mStartCond->isConform()) && (!isStop) ) //判断是否满足启动条件
        {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }

    if(isStop)
    {
        isStop = false;
        return ;
    }

    //发送时机
    auto type = mTiming->getType();

    if(timing::CYCLE == type) //循环发送
    {
        while(!isStop) //强制退出
        {
            //执行
            perform();

            //判断是否结束
            if(mStopCond)
            {
                if(mStopCond->isConform())
                    break;
            }
        }
    }
    else
    {
        //立即发送及手动发送执行一次(手动发送时设置启动条件)
        perform();
    }
}

void subFlow::perform()
{
    ///执行动作
    if(mAction.get() != nullptr)
    {
        toUi(mDescribe);

        mAction->exe();   
    }
}

void subFlow::toUi(const std::string &msg, bool state)
{
    Json::Value js;
    js["msgType"] = "showMsg";

    Json::Value tmp;
    tmp["record_uuid"] = mRecordUuid;

    if(state)
        tmp["status"] = "NORMAL";
    else
        tmp["status"] = "ERROR";

    tmp["show"] = "[" + QDateTime::currentDateTime().toString("hh:mm.ss.zzz").toStdString() + "]" + msg;

    js["msg"] = tmp;

    if(mUiAdapter)
        mUiAdapter->sendMsg(js.toStyledString().c_str(), js.toStyledString().size());
}
