#include "m1553bbusiness.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../src/PfCommon/crc/crc.h"
#include "../../src/PfIcdWorkBench/icdData/datastorage.h"
#include "../../virtualParams/virtualparams.h"

using namespace Pf::PfIcdWorkBench;

m1553bBusiness::m1553bBusiness():
    mBusObj(nullptr),
    mUiBus(nullptr),
    mCmdCnt(1)
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void m1553bBusiness::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{   
    //TODO:根据RT SA进行命令分类

    if( (3 == mRtAddr) && (8 == mSaAddr))
    {
        exeCmd(inData);
    }
}

void m1553bBusiness::exeCmd(const Pf::PfIcdWorkBench::byteArray &inData)
{
    if(inData.size() != 12)
        return;

    std::ostringstream strErr;

    dataStorage data;

    //crc
    unsigned short rcvCrc = data.getData(&inData.at(0), inData.size(), 10, 2, 0, 0);

    unsigned short calCrc = Pf::PfCommon::Crc::calCrc16(&inData.at(0), inData.size() - 2);

    if(rcvCrc != calCrc)
    {
        return ;
    }

    unsigned short rcvCnt = data.getData(&inData.at(0), inData.size(), 0, 2, 0, 0);

    if(mCmdCnt != rcvCnt)
    {
        unsigned short cmdCode = data.getData(&inData.at(0), inData.size(), 2, 2, 0, 0);;

        //更新参数
        mapKey vKey(mDevUuid.c_str(), "62010", std::to_string((int)cmdCode).c_str());

        virtualParams::getInstance()->setValue(vKey);

        mCmdCnt = rcvCnt;
    }
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

std::shared_ptr<business> m1553bBusiness::clone()
{
    m1553bBusiness *obj = new m1553bBusiness();

    std::shared_ptr<business> tmp(obj);

    return tmp;
}
