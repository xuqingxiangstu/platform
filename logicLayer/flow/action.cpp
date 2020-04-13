#include "action.h"

#include <chrono>
#include <thread>

#include <QDateTime>
#include "frame.h"

#include "../src/PfSql/paramsTable/sysinterfacetable.h"
#include "../../src/PfCommon/tools/ut_error.h"

//#define DEBUG_ACTION 1

action::action()
    :mTimeOut(0),
      mFrameObj(nullptr),
      mAdapterManagerObj(nullptr),
      mUiAdapter(nullptr)
{

}

action::~action()
{

}

void action::init(TiXmlElement *xmlEle)
{
    TiXmlElement *tmpEle = nullptr;

    tmpEle = xmlEle->FirstChildElement("dest_system_uuid");
    if(tmpEle)
    {
        const char *point = tmpEle->GetText();
        if(point)
            mDstSysUuid = std::string(point);
    }

    tmpEle = xmlEle->FirstChildElement("sleep");
    if(tmpEle)
    {
        const char *point = tmpEle->GetText();
        if(point)
            mTimeOut = std::atoi(point);
    }

    tmpEle = xmlEle->FirstChildElement("frame");
    if(tmpEle)
    {
        mFrameObj = std::make_shared<frame>();
        mFrameObj->init(tmpEle);
    }
#ifdef DEBUG_ACTION
#else    
    //校验帧类型
    if(mFrameObj == nullptr)
    {
        UT_THROW_EXCEPTION("mFrameObj == nullptr");
    }
#endif
}

bool action::exe()
{
    bool res = false;

#ifdef DEBUG_ACTION

    if(mTimeOut > 0)
        std::this_thread::sleep_for(std::chrono:: milliseconds (mTimeOut));

    std::vector<unsigned char> msg;

    if(mFrameObj)
        mFrameObj->getFrameMsg(msg);
#else
    //校验目标设备
    if(mAdapterManagerObj == nullptr)
    {
        UT_THROW_EXCEPTION("mAdapterManagerObj == nullptr");
    }

    if(!mAdapterManagerObj->isExist(mDstSysUuid))
    {
        UT_THROW_EXCEPTION("dev is not exist (" + mDstSysUuid + ")");
    }

    //step1：获取目标设备句柄
    Pf::PfAdapter::Adapter *obj;

    mAdapterManagerObj->getAdapter(mDstSysUuid, &obj);

    //step2：延时
    if(mTimeOut > 0)
        std::this_thread::sleep_for(std::chrono:: milliseconds (mTimeOut));

    //step3：获取组帧数据
    std::vector<unsigned char> msg;

    bool isAck = false;
    int rcvSize = 0;

    try
    {
        //如为需要确认帧则有重发机制，否则发送后不存在确认帧

        mFrameObj->getFrameMsg(msg, isAck);

        if(msg.size() == 0)
        {
            UT_SHOW("send msg size == 0");
            return res;
        }

        if(isAck)   //需要应答(400ms内没有收到则重发2次)
        {
            if(!obj->atomicTrMsg((const char*)&msg.at(0), msg.size(), (char*)mRcvBus, rcvSize, 400))
            {
                int resendCnt = 0;
                while(1)
                {
                    mFrameObj->getResendMsg(msg);                    

                    if(obj->atomicTrMsg((const char*)&msg.at(0), msg.size(), (char*)mRcvBus, rcvSize, 400))
                        break;

                    resendCnt++;

                    toUi("timeout resend " + std::to_string(resendCnt), false);

                    if(resendCnt == 2)
                    {
                        break;
                    }
                }
            }
            else
            {
                res = true;
            }
        }
        else    //不需要应答
        {
            //step4：发送
            res = obj->sendMsg((const char*)&msg.at(0), msg.size());
        }
    }
    catch(std::runtime_error err)
    {
        UT_THROW_EXCEPTION("[ERROR]" + std::string(err.what()));
    }
    catch(Json::LogicError err)
    {
        UT_SHOW(std::string(err.what()));
        UT_THROW_EXCEPTION("[ERROR]" + std::string(err.what()));
    }
    catch(...)
    {
        UT_THROW_EXCEPTION("[ERROR]");
    }

#endif

    return res;
}

bool action::isConform()
{
    return false;
}

void action::setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj)
{
    mAdapterManagerObj = adapterManagerObj;

    //获取发送UI句柄
    adapterManagerObj->getAdapter("UI", &mUiAdapter);
}

void action::setRecordUuid(std::string uuid)
{
    mRecordUuid = uuid;
}

Json::Value action::getRunItems()
{
    Json::Value tmpJs;

    tmpJs["delay"] = mTimeOut;

    Json::Value sysJs = sysInterfaceTable::getInstance()->getValue(mDstSysUuid);

    if(!sysJs.isNull())
    {
        tmpJs["dest_system_name"] = sysJs[SYSTEM_INTERFACE_TABLE_DEV_NAME].asString();
        tmpJs["dest_system_uuid"] = sysJs[SYSTEM_INTERFACE_TABLE_UUID].asString();
    }

    if(mFrameObj)
    {
        if(frame::Cmd == mFrameObj->getFrameType())
            tmpJs["cmdOrParam"] = "cmd";
        else if(frame::Param == mFrameObj->getFrameType())
            tmpJs["cmdOrParam"] = "param";
        tmpJs["info"] = mFrameObj->getRunItems();
    }

    return tmpJs;
}

void action::setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter)
{
    if(mFrameObj)
        mFrameObj->setIcdAdapter(icdAdapter);
}

void action::toUi(const std::string &msg, bool state)
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


/***********************************/

