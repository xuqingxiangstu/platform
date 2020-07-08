#include "c3business.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../src/PfCommon/crc/crc.h"
#include "../../virtualParams/virtualparams.h"

#include <sstream>

using namespace Pf::PfIcdWorkBench;

c3Business::c3Business():
    mBusObj(nullptr),
    mUiBus(nullptr)
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void c3Business::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{
    int table = result["table"].asInt();   //特征字
    std::ostringstream showMsg;
    byteArray sendMsg;

    std::copy(inData.begin(), inData.end(), std::back_inserter(sendMsg));

    Json::Value msgArray = result["msg"];

    if( !(msgArray.isArray() && (msgArray.size() > 0)) )
        return ;

    //回令
    if(1 == table)
    {        
        //编码
        int coding = msgArray[0]["coding"].asInt();
        if(coding > testName.size())
            return ;

        int flag = msgArray[0]["flag"].asInt();

        bool res = false;
        if(0x70 == flag)
            res = true;
        else
            res = false;

        toUi(testName.at(coding).toStdString(), res);
    }
    else if(2 == table)   //心跳包
    {

    }
    else if(3 == table)
    {
        showMsg.str("");
        showMsg << std::endl;
        for(int index = 0; index < msgArray.size(); index++)
        {
            Json::Value tmpV = msgArray[index];

            int coding = tmpV["coding"].asInt();
            if(coding > table3VoltageName.size())
                return ;

            int flag = tmpV["flag"].asInt();

            float value = tmpV["value"].asFloat();

            std::string res = "异常";
            if(0x70 == flag)
                res = "正常";
            else
                res = "异常";

            showMsg << table3VoltageName.at(coding).toStdString() << "(" << std::to_string(value) << ") - " << res << std::endl;
        }

        //toUi(showMsg.str());
    }
    else if(4 == table)
    {
        showMsg.str("");
        showMsg << std::endl;
        for(int index = 0; index < msgArray.size(); index++)
        {
            Json::Value tmpV = msgArray[index];

            int coding = tmpV["coding"].asInt();
            if(coding > table4VoltageName.size())
                return ;

            int flag = tmpV["flag"].asInt();

            int value = tmpV["value"].asInt();

            std::string res = "工作状态";
            if(0x0 == value)
                res = "工作状态";
            else
                res = "保险状态";

            showMsg << table4VoltageName.at(coding).toStdString() << "(" << std::to_string(value) << ") - " << res << std::endl;
        }

        //toUi(showMsg.str());
    }
}



void c3Business::exeCmd(Json::Value valueJs)
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

    respondCmd(0x2131, 0xB1, array);
}

void c3Business::testInfo(Json::Value valueJs)
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

    respondCmd(0x2132, 0xB2, array);
}

void c3Business::queryVersion(Json::Value regionJs)
{

    //回令
    Json::Value array;
    array.append(0x1);
    array.append(0x2);
    array.append(0x3);

    respondCmd(0x2133, 0xB3, array);
}

void c3Business::sendFsCmd(Json::Value regionJs)
{
    //回令
    Json::Value array;
    array.append(0xEE);

    respondCmd(0x2135, 0x85, array);
}

void c3Business::checkCmd(Json::Value regionJs)
{

    //回令
    Json::Value array;
    array.append(0x1);
    array.append(0x1);
    for(int index = 0; index < 5; index++)
        array.append(0);

    respondCmd(0x2135, 0xB5, array);
}


void c3Business::respondCmd(int frameType, int code, Json::Value array)
{
    if("frameCZXK" != mFrameObj->getFrameName())
        return ;

    Json::Value cmdJs;
    cmdJs["frame_type"] = frameType;
    cmdJs["code"] = code;
    cmdJs["msg"] = array;

    byteArray msg;
    mFrameObj->simulation(msg, cmdJs);

    if(mBusObj && (msg.size() > 0))
    {
        mBusObj->sendMsg((const char*)&msg.at(0), msg.size());
    }
}

bool c3Business::getTableAndCoding(int type, std::string &table, std::string &coding)
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

std::string c3Business::getCmdName(int type)
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

void c3Business::setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort)
{
    mDstIp = dstIp;
    mDstPort = dstPort;
    mBusObj = obj;
}

void c3Business::setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj)
{
    mFrameObj = obj;
}

void c3Business::toUi(const std::string &msg, bool state)
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
std::shared_ptr<business> c3Business::clone()
{
    c3Business *obj = new c3Business();

    std::shared_ptr<business> tmp(obj);

    return tmp;
}
