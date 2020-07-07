#include "action.h"

#include <chrono>
#include <thread>

#include <QDateTime>
#include "frame.h"
#include <QDebug>

#include "../logRecord/logmanager.h"
#include "../custom/custommsg.h"
#include "../src/PfSql/paramsTable/sysinterfacetable.h"
#include "../../src/PfCommon/tools/ut_error.h"
#include "../src/PfAdapter/m1553Adapter/m1553adapter.h"
#include "../virtualParams/virtualparams.h"

//#define DEBUG_ACTION 1

action::action(QObject *parent)
    :QObject(parent),
      mTimeOut(0),
      mFrameObj(nullptr),
      mAdapterManagerObj(nullptr),
      mUiAdapter(nullptr)
{
    QObject::connect(this, &action::record, logManager::getInstance().get(), &logManager::record);
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
        mFrameObj->setDevUuid(mDstSysUuid);
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

    //modify xqx 20200416自定义消息处理（特殊消息处理）
    if(frame::CustomMsg == mFrameObj->getMsgType())
    {
        std::string table;
        std::string coding;
        mFrameObj->getCustomMsg(table, coding);
        auto icdObj = mFrameObj->getCurFrameObj();        

        if(UI_CUSTOM_MSG == table) //界面内容消息触发
        {
            //更新参数
            mapKey vKey("UI", UI_CUSTOM_MSG, coding.c_str());

            virtualParams::getInstance()->setValue(vKey);
        }
        else if(XK_CUSTOM_MSG == table)
        {
            customMsg::getInstance()->exe(mFrameObj, mRecordUuid, icdObj, obj, mUiAdapter,table, coding);
        }

        return true;
    }

    //modify xqx 2020-4-26 20:14:50 1553B时发送前需设置RT地址及SA地址

    if(mFrameObj->is1553B() && ("m1553Adapter" == obj->getClassName()))
    {
        std::string model = mFrameObj->get1553BModel();
        std::string bus = mFrameObj->get1553BBus();

        if("A" == bus)
            dynamic_cast<Pf::PfAdapter::m1553Adapter*>(obj)->set1553BBus(Pf::PfAdapter::m1553Adapter::Bus_A);
        else
            dynamic_cast<Pf::PfAdapter::m1553Adapter*>(obj)->set1553BBus(Pf::PfAdapter::m1553Adapter::Bus_B);

        if("BC->RT" == model)
        {
            int rtAddr = 0;
            int saAddr = 0;

            mFrameObj->getBcModelInfo(rtAddr, saAddr);
            dynamic_cast<Pf::PfAdapter::m1553Adapter*>(obj)->set1553BModel(Pf::PfAdapter::m1553Adapter::BC_2_RT);
            dynamic_cast<Pf::PfAdapter::m1553Adapter*>(obj)->setBc2RtInfo(rtAddr, saAddr);
        }
        else if("RT->RT" == model)
        {
            int sRt = 0;
            int sSa = 0;
            int rRt = 0;
            int rSa = 0;
            int dataSize = 0;

            mFrameObj->getRtModelInfo(sRt, sSa, rRt, rSa, dataSize);

            dynamic_cast<Pf::PfAdapter::m1553Adapter*>(obj)->set1553BModel(Pf::PfAdapter::m1553Adapter::RT_2_RT);
            dynamic_cast<Pf::PfAdapter::m1553Adapter*>(obj)->setRt2RtInfo(sRt, sSa, rRt, rSa, dataSize);
        }
    }

    //end

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
                toLog((const char*)&msg.at(0), msg.size());

                int resendCnt = 0;
                while(1)
                {
                    qDebug() << "[" << QDateTime::currentDateTime().toString("hh:mm.ss.zzz") << "]" << "resend msg";
                    mFrameObj->getResendMsg(msg);                    

                    if(obj->atomicTrMsg((const char*)&msg.at(0), msg.size(), (char*)mRcvBus, rcvSize, 400))
                    {
                        toLog((const char*)&msg.at(0), msg.size());

                        break;
                    }

                    toLog((const char*)&msg.at(0), msg.size());

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

                toLog((const char*)&msg.at(0), msg.size());
            }
        }
        else    //不需要应答
        {
            //step4：发送
            res = obj->sendMsg((const char*)&msg.at(0), msg.size());

            toLog((const char*)&msg.at(0), msg.size());
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


    return res;
}

void action::toLog(const char *msg, const int &msgSize)
{
    //modify xqx 2020-6-2 10:52:28 二进制源码转换为十六进制字符串+时间
    QByteArray recordMsg;
    recordMsg += "[";
    recordMsg += QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    recordMsg += "]";
    recordMsg += "[s]->";

    recordMsg += QByteArray((char*)msg, msgSize).toHex();
    recordMsg += "\n";

    emit record(QString::fromStdString(mDstSysUuid), recordMsg);

    //end

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

