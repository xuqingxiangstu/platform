#include "s93business.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../../src/PfCommon/crc/crc.h"
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

    unsigned int frameType = result["frame_type"].asUInt();

    Json::Value regionValue = result["region"];


    Json::Value array = regionValue["data"];

    Json::Value table = regionValue["table_num"];

    for(int index = 0; index < array.size(); index++)
    {
        int coding = array[index]["coding"].asInt();

        mapKey vKey(mDevUuid.c_str(), QString::number(table.asInt(), 16), std::to_string(coding).c_str());

        virtualParams::getInstance()->setValue(vKey, array[index]["value"]);

        qDebug() << "[93]->" << mDevUuid.c_str() << + ":" << QString::number(table.asInt(), 16) << "," << QString::number(coding) << "," << array[index]["value"].asString().c_str();
    }
}

std::string s93Business::getVirtualCoding(unsigned char table)
{
    std::string coding = "";
    //根据EXcel配置映射关系

    switch(table)
    {
    case 0x43:
        coding = "0";
        break;
    case 0x81:
        coding = "1";
        break;
    case 0x8A:
        coding = "2";
        break;
    case 0x8C:
        coding = "3";
        break;
    case 0x8F:
        coding = "4";
        break;
    case 0x90:
        coding = "5";
        break;
    case 0x91:
        coding = "6";
        break;
    case 0x92:
        coding = "7";
        break;
    case 0x93:
        coding = "8";
        break;
    case 0xC5:
        coding = "9";
        break;
    }

    return coding;
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
std::shared_ptr<business> s93Business::clone()
{
    s93Business *obj = new s93Business();

    std::shared_ptr<business> tmp(obj);

    return tmp;
}
