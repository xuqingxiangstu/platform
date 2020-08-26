#include "czxkbusiness.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../../src/PfCommon/crc/crc.h"
#include "../../virtualParams/virtualparams.h"


using namespace Pf::PfIcdWorkBench;

czxkBusiness::czxkBusiness():
    mBusObj(nullptr),
    mUiBus(nullptr)
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void czxkBusiness::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{
    int cmdType = result["cmd_code"].asInt();   //特征字
    Json::Value regionJs = result["region"];

    byteArray sendMsg;

    std::copy(inData.begin(), inData.end(), std::back_inserter(sendMsg));    

    //回令
    switch(cmdType)
    {
    case 0xB1:
        exeCmd(regionJs, result["dst_num"].asInt(), result["src_num"].asInt());
        break;
    case 0xB2:
        testInfo(regionJs, result["dst_num"].asInt(), result["src_num"].asInt());
        break;
    case 0xB3:
        queryVersion(regionJs, result["dst_num"].asInt(), result["src_num"].asInt());
        break;
    case 0xB5:
        checkCmd(regionJs, result["dst_num"].asInt(), result["src_num"].asInt());
        break;   
    case 0x85:
        sendFsCmd(regionJs, result["dst_num"].asInt(), result["src_num"].asInt());
        break;
    }

}

void czxkBusiness::exeCmd(Json::Value valueJs, unsigned char srcNode, unsigned char dstNode)
{
    Json::Value regionJs = valueJs["data"];

    if( !((regionJs.isArray()) && regionJs.size() > 1))
        return ;

    int testNum = regionJs[1]["value"].asInt();

    if(testNum > testName.size())
        return ;

    //显示
    toUi(testName.at(testNum).toStdString());

    //回令
    Json::Value array;
    for(int index = 0; index < 7; index++)
        array.append(0);

    respondCmd(0x2131, 0xB1, array, srcNode, dstNode);

    //更新参数

    mapKey vKey(mDevUuid.c_str(), mViratulTableNum.c_str(), std::to_string(testNum).c_str());

    virtualParams::getInstance()->setValue(vKey);
}

void czxkBusiness::testInfo(Json::Value valueJs, unsigned char srcNode, unsigned char dstNode)
{
    Json::Value regionJs = valueJs["data"];

    if( !((regionJs.isArray()) && regionJs.size() > 1))
        return ;

    int keyValue = regionJs[1]["value"].asInt();

    //显示
    toUi(regionJs[1]["value"].asString());

    //回令
    Json::Value array;
    array.append(keyValue);
    for(int index = 0; index < 6; index++)
        array.append(0);

    respondCmd(0x2132, 0xB2, array, srcNode, dstNode);
}

void czxkBusiness::queryVersion(Json::Value regionJs, unsigned char srcNode, unsigned char dstNode)
{

    //回令
    Json::Value array;
    array.append(0x1);
    array.append(0x2);
    array.append(0x3);

    respondCmd(0x2133, 0xB3, array, srcNode, dstNode);
}

void czxkBusiness::sendFsCmd(Json::Value regionJs, unsigned char srcNode, unsigned char dstNode)
{
    //回令
    Json::Value array;
    array.append(0xEE);

    respondCmd(0x2135, 0x85, array, srcNode, dstNode);
}

void czxkBusiness::checkCmd(Json::Value regionJs, unsigned char srcNode, unsigned char dstNode)
{

    //回令
    Json::Value array;
    array.append(0x1);
    array.append(0x1);
    for(int index = 0; index < 5; index++)
        array.append(0);

    respondCmd(0x2135, 0xB5, array, srcNode, dstNode);
}


void czxkBusiness::respondCmd(int frameType, int code, Json::Value array, unsigned char srcNode, unsigned char dstNode)
{
    if("frameCZXK" != mFrameObj->getFrameName())
        return ;

    Json::Value cmdJs;
    cmdJs["frame_type"] = frameType;
    cmdJs["src_node"] = srcNode;
    cmdJs["dst_node"] = dstNode;
    cmdJs["code"] = code;
    cmdJs["msg"] = array;

    byteArray msg;
    mFrameObj->simulation(msg, cmdJs);

    if(mBusObj && (msg.size() > 0))
    {
        mBusObj->sendMsg((const char*)&msg.at(0), msg.size());
    }
}

bool czxkBusiness::getTableAndCoding(int type, std::string &table, std::string &coding)
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

std::string czxkBusiness::getCmdName(int type)
{
    std::string name = "";
    switch(type)
    {
    case 0x30:
        name = "通讯检查命令报文";
        break;
    case 0x36:
        name = "位置信息点装订或已知点修正报文";
        break;
    }
    return name;
}

void czxkBusiness::setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort)
{
    mDstIp = dstIp;
    mDstPort = dstPort;
    mBusObj = obj;
}

void czxkBusiness::setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj)
{
    mFrameObj = obj;
}

void czxkBusiness::toUi(const std::string &msg, bool state)
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
std::shared_ptr<business> czxkBusiness::clone()
{
    czxkBusiness *obj = new czxkBusiness();

    std::shared_ptr<business> tmp(obj);

    return tmp;
}
