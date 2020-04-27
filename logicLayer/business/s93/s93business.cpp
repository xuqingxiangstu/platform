#include "s93business.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../src/PfCommon/crc/crc.h"
#include "../../virtualParams/virtualparams.h"

using namespace Pf::PfIcdWorkBench;

s93Business::s93Business():
    mBusObj(nullptr),
    mUiBus(nullptr)
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void s93Business::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{

    if(result.isNull())
        return;

    int frameType = result["frame_type"].asInt();

    if(0x43 == frameType)
    {
        toUi("接收到时间同步命令");
    }
    else if(0xC5 == frameType)
    {
        toUi("清除状态命令");
    }
    //

}

void s93Business::setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort)
{
    mDstIp = dstIp;
    mDstPort = dstPort;
    mBusObj = obj;
}

void s93Business::setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj)
{
    mFrameObj = obj;
}

void s93Business::toUi(const std::string &msg, bool state)
{
    Json::Value js;
    js["msgType"] = "showMsg";

    Json::Value tmp;
    tmp["record_uuid"] = mRecordUuid;

    if(state)
        tmp["status"] = "NORMAL";
    else
        tmp["status"] = "ERROR";

    tmp["show"] = "[" + QDateTime::currentDateTime().toString("hh:mm.ss.zzz").toStdString() + "]" + "-> " + msg;

    js["msg"] = tmp;

    if(mUiBus)
        mUiBus->sendMsg(js.toStyledString().c_str(), js.toStyledString().size());

}
