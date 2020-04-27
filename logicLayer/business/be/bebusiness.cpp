#include "bebusiness.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../src/PfCommon/crc/crc.h"
#include "../../virtualParams/virtualparams.h"

using namespace Pf::PfIcdWorkBench;

beBusiness::beBusiness():
    mBusObj(nullptr),
    mUiBus(nullptr)
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void beBusiness::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{
    byteArray askMsg;

    //判断是否应答
    if(mFrameObj->getAskMsg(inData, askMsg, result))
    {
        mBusObj->sendMsg((const char*)&askMsg.at(0), askMsg.size());
    }

    //更新参数
    Json::Value newJs;
    if(mFrameObj->getValidValue(result, newJs))
    {
        for(int index = 0; index < newJs.size(); index++)
        {
            int coding = 0, table = 0;
            if(!newJs[index]["coding"].isNull())
            {
                coding = newJs[index]["coding"].asInt();
            }

            if(!newJs[index]["table"].isNull())
            {
                table = newJs[index]["table"].asInt();
            }

            virtualParams::getInstance()->setValue({mDevUuid, std::to_string(table), std::to_string(coding)}, mapValue());
        }
    }
}


void beBusiness::setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort)
{
    mDstIp = dstIp;
    mDstPort = dstPort;
    mBusObj = obj;
}

void beBusiness::setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj)
{
    mFrameObj = obj;
}

void beBusiness::toUi(const std::string &msg, bool state)
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
