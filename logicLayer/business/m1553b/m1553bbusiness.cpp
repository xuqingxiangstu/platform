#include "m1553bbusiness.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../src/PfCommon/crc/crc.h"
#include "../../virtualParams/virtualparams.h"

using namespace Pf::PfIcdWorkBench;

m1553bBusiness::m1553bBusiness():
    mBusObj(nullptr),
    mUiBus(nullptr)
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void m1553bBusiness::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{
    if(result.isNull())
        return;

    //TODO:根据RT SA进行命令分类
}

void m1553bBusiness::setParam(const Json::Value &param)
{
    if(param.isNull() || param["rt"].isNull() || param["sa"].isNull())
        return ;

    mRtAddr = param["rt"].asInt();
    mSaAddr = param["sa"].asInt();
}

void m1553bBusiness::setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort)
{
    mDstIp = dstIp;
    mDstPort = dstPort;
    mBusObj = obj;
}

void m1553bBusiness::setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj)
{
    mFrameObj = obj;
}

void m1553bBusiness::toUi(const std::string &msg, bool state)
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
