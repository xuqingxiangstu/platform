#include "dmbusiness.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../../src/PfCommon/crc/crc.h"
#include "../../virtualParams/virtualparams.h"

using namespace Pf::PfIcdWorkBench;

dmBusiness::dmBusiness():
    mBusObj(nullptr),
    mUiBus(nullptr)
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void dmBusiness::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{
    unsigned int cmdType = result["cmd_code"].asInt();   //特征字

    //界面显示
    std::string cmdName = getCmdName(cmdType);

    toUi(cmdName);

    byteArray sendMsg;

    std::copy(inData.begin(), inData.end(), std::back_inserter(sendMsg));

    //回令
    switch(cmdType)
    {
    case 0x30:
        sendMsg[0] = 0xA0;
        sendMsg[sendMsg.size() - 1] = Pf::PfCommon::Crc::xOr(&sendMsg.at(0), sendMsg.size() - 1);
        mBusObj->sendMsg((const char*)&sendMsg.at(0), sendMsg.size());
        break;
    case 0x36:
        sendMsg[0] = 0xA6;
        sendMsg[sendMsg.size() - 1] = Pf::PfCommon::Crc::xOr(&sendMsg.at(0), sendMsg.size() - 1);
        mBusObj->sendMsg((const char*)&sendMsg.at(0), sendMsg.size());
        break;
    case 0x31:
        sendMsg[0] = 0xA1;
        sendMsg[sendMsg.size() - 1] = Pf::PfCommon::Crc::xOr(&sendMsg.at(0), sendMsg.size() - 1);
        mBusObj->sendMsg((const char*)&sendMsg.at(0), sendMsg.size());
        break;
    case 0x52:
        sendMsg[0] = 0xC2;
        sendMsg[sendMsg.size() - 1] = Pf::PfCommon::Crc::xOr(&sendMsg.at(0), sendMsg.size() - 1);
        mBusObj->sendMsg((const char*)&sendMsg.at(0), sendMsg.size());
        break;
    case 0x39:
        sendMsg[0] = 0xA9;
        sendMsg[sendMsg.size() - 1] = Pf::PfCommon::Crc::xOr(&sendMsg.at(0), sendMsg.size() - 1);
        mBusObj->sendMsg((const char*)&sendMsg.at(0), sendMsg.size());
        break;
    case 0x5B:
        sendMsg[0] = 0xCB;
        sendMsg[sendMsg.size() - 1] = Pf::PfCommon::Crc::xOr(&sendMsg.at(0), sendMsg.size() - 1);
        mBusObj->sendMsg((const char*)&sendMsg.at(0), sendMsg.size());
        break;
    case 0x3A:
        sendMsg[0] = 0xAA;
        sendMsg[sendMsg.size() - 1] = Pf::PfCommon::Crc::xOr(&sendMsg.at(0), sendMsg.size() - 1);
        mBusObj->sendMsg((const char*)&sendMsg.at(0), sendMsg.size());
        break;
    case 0x53:
        sendMsg[0] = 0xC3;
        sendMsg[sendMsg.size() - 1] = Pf::PfCommon::Crc::xOr(&sendMsg.at(0), sendMsg.size() - 1);
        mBusObj->sendMsg((const char*)&sendMsg.at(0), sendMsg.size());
        break;
    default:
        sendMsg.clear();
        sendMsg = {0xFF, 0x4, 0x2, 0x0};
        sendMsg[sendMsg.size() - 1] = Pf::PfCommon::Crc::xOr(&sendMsg.at(0), sendMsg.size() - 1);
        mBusObj->sendMsg((const char*)&sendMsg.at(0), sendMsg.size());
        break;
    }

#if 0
    //参数更新
    std::string tableNum, coding;

    if(getTableAndCoding(cmdType, tableNum, coding));
        virtualParams::getInstance()->setValue({mDevUuid, tableNum, coding});
#endif
}

bool dmBusiness::getTableAndCoding(int type, std::string &table, std::string &coding)
{
    bool res = false;

    switch(type)
    {
    case 0x30:
        table = "50000";
        coding = "1";
        res = true;
        break;
    }

    return res;
}

std::string dmBusiness::getCmdName(unsigned int type)
{
    std::string name = "";
    switch(type)
    {
    case 0x30:
        name = "通讯检查命令";
        break;
    case 0x31:
        name = "初始对准命令";
        break;
    case 0x52:
        name = "停止初始对准命令";
        break;
    case 0x39:
        name = "惯性组合导航命令";
        break;
    case 0x5B:
        name = "卫星组合导航命令";
        break;
    case 0x3A:
        name = "地标点修正值命令";
        break;
    case 0x53:
        name = "地标拾取仪天线工作命令";
        break;
    case 0x36:
        name = "位置信息点装订或已知点修正报文";
        break;
    }
    return name;
}

void dmBusiness::setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort)
{
    mDstIp = dstIp;
    mDstPort = dstPort;
    mBusObj = obj;
}

void dmBusiness::setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj)
{
    mFrameObj = obj;
}

void dmBusiness::toUi(const std::string &msg, bool state)
{
    Json::Value js;
    js["msgType"] = "showMsg";

    Json::Value tmp;
    tmp["record_uuid"] = mRecordUuid;

    if(state)
        tmp["status"] = "NORMAL";
    else
        tmp["status"] = "ERROR";

    tmp["show"] = "[" + QDateTime::currentDateTime().toString("hh:mm.ss.zzz").toStdString() + "]" + "[信息控制系统]->" + msg;

    js["msg"] = tmp;

    if(mUiBus)
        mUiBus->sendMsg(js.toStyledString().c_str(), js.toStyledString().size());

}

std::shared_ptr<business> dmBusiness::clone()
{
    dmBusiness *obj = new dmBusiness();

    std::shared_ptr<business> tmp(obj);

    return tmp;
}

