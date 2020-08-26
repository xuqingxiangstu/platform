#include "bebusiness.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../../src/PfCommon/crc/crc.h"
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

    //更新信息字格式1、3参数
    Json::Value newJs;
    int infoType = -1;
    if(mFrameObj->getValidValue(result, newJs, infoType))
    {
        if(infoType != 1)
        {
            for(int index = 0; index < newJs.size(); index++)
            {
                int coding = 0, table = 0;
                Json::Value value = Json::Value();
                if(!newJs[index]["coding"].isNull())
                {
                    coding = newJs[index]["coding"].asInt();
                }

                if(!newJs[index]["table"].isNull())
                {
                    table = newJs[index]["table"].asInt();
                }

                if(!newJs[index]["value"].isNull())
                {
                    value = newJs[index]["value"];
                }

    #ifndef QT_NO_DEBUG
                qDebug() << "[BE]->" << mDevUuid.c_str() << "," << QString::number(table) << "," << QString::number(coding) << "," << value.asString().c_str();
    #endif

                mapKey vKey(mDevUuid.c_str(), std::to_string(table).c_str(), std::to_string(coding).c_str());

                virtualParams::getInstance()->setValue(vKey, value);
            }
        }
    }

    //更新信息字格式2参数
    if(1 == infoType)
    {
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

                    mapKey vKey(mDevUuid.c_str(), QString::number(table, 10), std::to_string(coding).c_str());

                    virtualParams::getInstance()->setValue(vKey, array[index]["value"]);
    #ifndef QT_NO_DEBUG
                    qDebug() << "[BE]->" << mDevUuid.c_str() << + ":" << QString::number(table, 10) << "," << QString::number(coding) << "," << array[index]["value"].asString().c_str();
    #endif
                }
            }
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
std::shared_ptr<business> beBusiness::clone()
{
    beBusiness *obj = new beBusiness();

    std::shared_ptr<business> tmp(obj);

    return tmp;
}
