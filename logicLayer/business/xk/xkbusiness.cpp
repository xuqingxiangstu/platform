#include "xkbusiness.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>
using namespace Pf::PfIcdWorkBench;

xkBusiness::xkBusiness():
    mBusObj(nullptr),
    mUiBus(nullptr)
{    
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void xkBusiness::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{
    int frameType = result["frame_type"].asInt();   //特征字

    if(frameType >= ZY_STAT && frameType <= ZY_STOP)  //诸元处理
    {

    }
    else if(0xAF == frameType)//心跳
    {
        byteArray msg;
        if(mFrameObj)
        {
            Json::Value info;
            info["frame_type"] = 0xAF;
            info["sub_frame_type"] = 0xAF;

            mFrameObj->simulation(msg, info);
        }
        if(mBusObj && (msg.size() > 0))
        {
            mBusObj->sendMsg((const char*)&msg.at(0), msg.size());
        }
    }
    else if(0x61 == frameType)//发射进程
    {
        testFlow(result);
    }
}

void xkBusiness::setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort)
{
    mDstIp = dstIp;
    mDstPort = dstPort;
    mBusObj = obj;    
}

void xkBusiness::setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj)
{
    mFrameObj = obj;
}

std::shared_ptr<business> xkBusiness::clone()
{

}

void xkBusiness::zy(const Json::Value &result)
{
    int subType = result["sub_frame_type"].asInt(); //子帧类型
    switch(subType)
    {
    case ZY_DOWN_FILE_TYPE:
        break;
    case ZY_FILE_INFO_TYPE:
        fileInfo(result);
        break;
    case ZY_REQUEST_DATA_TYPE:
        requestData(result);
        break;
    case ZY_TRANS_OVER_TYPE:
        break;
    case ZY_DATA_TYPE:
        fileData(result);
        break;
    }
}

void xkBusiness::fileInfo(const Json::Value &result)
{

}

void xkBusiness::requestData(const Json::Value &result)
{

}

void xkBusiness::fileData(const Json::Value &result)
{

}

std::string xkBusiness::getFlowName(int subType)
{
    std::string flowName = "";

    switch(subType)
    {
    case 0x91:
        flowName = "进入发射流程";
        break;
    case 0x92:
        flowName = "弹上设备加电";
        break;
    case 0x95:
        flowName = "上传单机、点位诸元";
        break;
    case 0x96:
        flowName = "基准图传递";
        break;
    case 0x97:
        flowName = "上传被动雷达库信息";
        break;
    case 0x98:
        flowName = "上传弹道诸元";
        break;
    case 0xA4:
        flowName = "上传引控诸元";
        break;
    case 0x99:
        flowName = "瞄准结束";
        break;
    case 0x9B:
        flowName = "导弹准备起竖";
        break;
    case 0x9E:
        flowName = "开栓";
        break;
    case 0xA1:
        flowName = "电池激活";
        break;
    case 0xA2:
        flowName = "转电";
        break;
    case 0xA5:
        flowName = "允许伺服电池激活";
        break;
    case 0xA3:
        flowName = "允许发射";
        break;
    case 0xA6:
        flowName = "上传战训测控诸元";
        break;
    }

   return flowName;
}

void xkBusiness::testFlow(const Json::Value &result)
{
    bool isError = false;

    int subType = result["sub_frame_type"].asInt();
    int data = result["data"].asInt();

    0xAA == data ? isError = false : isError = true;

    std::string name = getFlowName(subType);

    if(name != "")
        toUi(name, !isError);
}

void xkBusiness::toUi(const std::string &msg, bool state)
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
