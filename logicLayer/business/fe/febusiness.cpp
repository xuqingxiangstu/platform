#include "febusiness.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>
#include <QString>
#include "../../src/PfCommon/crc/crc.h"
#include "../../virtualParams/virtualparams.h"

#include "../../src/PfCommon/crc/crc.h"
#include "../../src/PfSql/paramsTable/udptable.h"
#include "../../src/PfSql/paramsTable/sysinterfacetable.h"

using namespace Pf::PfIcdWorkBench;

feBusiness::feBusiness():
    mBusObj(nullptr),
    mUiBus(nullptr)
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void feBusiness::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{
    byteArray askMsg;

    //判断是否应答
    if(mFrameObj->getAskMsg(inData, askMsg, result))
    {
        mBusObj->sendMsg((const char*)&askMsg.at(0), askMsg.size());
    }
    //更新参数
    //qDebug() << result.toStyledString().c_str();
    if(!result["region"].isNull())
    {
        Json::Value regionValue = result["region"];

        if(!regionValue["table_num"].isNull())
        {
            unsigned int table = regionValue["table_num"].asUInt();

            Json::Value array = regionValue["data"];
            for(int index = 0; index < array.size(); index++)
            {
                int coding = array[index]["coding"].asInt();

                mapKey vKey(mDevUuid.c_str(), QString::number(table, 16), std::to_string(coding).c_str());

                virtualParams::getInstance()->setValue(vKey, array[index]["value"]);

                qDebug() << "[FE]->" << mDevUuid.c_str() << + ":" << QString::number(table, 16) << "," << QString::number(coding) << "," << array[index]["value"].asString().c_str();
            }
        }
    }

    //帧类型
    Json::Value headJs = result["head"];

    int frameType = headJs["frame_type"].asInt();

    int headSize = 16;

    /*if(0x0206 == frameType) //6.4.2.4　初始数据装订指令
    {
        Pf::PfIcdWorkBench::byteArray convertMsg;
        std::copy(inData.begin(), inData.end(), std::back_inserter(convertMsg));

        if(convertMsg.size() > 9)
        {
            unsigned char srcNode = convertMsg[5];
            unsigned char dstNode = convertMsg[6];
            unsigned char sendSys = convertMsg[7];
            unsigned char rcvSys = convertMsg[8];

            convertMsg[5] = dstNode;
            convertMsg[6] = srcNode;
            convertMsg[7] = rcvSys;
            convertMsg[8] = sendSys;
        }

        //更新CRC
        unsigned short crc = Pf::PfCommon::Crc::calCrc16(&convertMsg.at(3), convertMsg.size() - 3);
        convertMsg.at(1) = crc & 0xFF;
        convertMsg.at(2) = crc >> 8;

        mBusObj->sendMsg((const char*)&convertMsg.at(0), convertMsg.size());
    }*/
}

void feBusiness::respondCmd(int frameType, Json::Value head, Json::Value array)
{
    Json::Value cmdJs;
    cmdJs["frame_type"] = frameType;
    cmdJs["head_src_node"] = head["head_src_node"];
    cmdJs["head_dst_node"] = head["head_dst_node"];
    cmdJs["head_send_sys"] = head["head_send_sys"];
    cmdJs["head_rcv_sys"] = head["head_rcv_sys"];
    cmdJs["msg"] = array;

    byteArray msg;
    mFrameObj->simulation(msg, cmdJs);

    if(mBusObj && (msg.size() > 0))
    {        
        mBusObj->sendMsg((const char*)&msg.at(0), msg.size());
    }
}


void feBusiness::setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort)
{
    mDstIp = dstIp;
    mDstPort = dstPort;
    mBusObj = obj;
}

void feBusiness::setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj)
{
    mFrameObj = obj;
}

void feBusiness::toUi(const std::string &msg, bool state)
{
    Json::Value js;
    js["msgType"] = "showMsg";

    Json::Value tmp;
    tmp["record_uuid"] = mRecordUuid;

    if(state)
        tmp["status"] = "NORMAL";
    else
        tmp["status"] = "ERROR";

    tmp["show"] = "[" + QDateTime::currentDateTime().toString("hh:mm.ss.zzz").toStdString() + "]" + "[FE业务]->" + msg;

    js["msg"] = tmp;

    if(mUiBus)
        mUiBus->sendMsg(js.toStyledString().c_str(), js.toStyledString().size());

}

std::shared_ptr<business> feBusiness::clone()
{
    feBusiness *obj = new feBusiness();

    std::shared_ptr<business> tmp(obj);

    return tmp;
}
