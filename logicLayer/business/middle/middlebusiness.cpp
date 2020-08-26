#include "middlebusiness.h"

#include "../../uibus.h"
#include <QDebug>
#include <QDateTime>

#include "../../../src/PfSql/paramsTable/sysinterfacetable.h"
#include "../../../src/PfSql/paramsTable/udptable.h"
#include "../../../src/PfCommon/crc/crc.h"
#include "../../virtualParams/virtualparams.h"

using namespace Pf::PfIcdWorkBench;

middleBusiness::middleBusiness():
    mBusObj(nullptr),
    mUiBus(nullptr)
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void middleBusiness::deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &result)
{
    byteArray askMsg;

    //判断是否应答
    if(mFrameObj->getAskMsg(inData, askMsg, result))
    {
        mBusObj->sendMsg((const char*)&askMsg.at(0), askMsg.size());
    }

    //更新参数
    Json::Value newJs;
    int infoType = -1;
    if(mFrameObj->getValidValue(result, newJs, infoType))
    {
        if(infoType != 1)
        {
            for(int index = 0; index < newJs.size(); index++)
            {
                Json::Value value = Json::Value();
                int coding = 0, table = 0;
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
                else
                {
                    continue;
                }

    #ifndef QT_NO_DEBUG
                qDebug() << "[middle]->" << mDevUuid.c_str() << + ":"  << "," << QString::number(table) << "," << QString::number(coding) << "," << value.asString().c_str();
    #endif
                mapKey vKey(mDevUuid.c_str(), std::to_string(table).c_str(), std::to_string(coding).c_str());

                virtualParams::getInstance()->setValue(vKey, value);
            }
        }
    }
    //界面显示信息,其它业务逻辑

    if(newJs.isNull())
        return ;

    if(result["region"].isNull())
        return ;

    int tableNum = result["region"]["table_num"].asInt();
    switch(tableNum)
    {
    case 4283:
        zy(result["region"]["data"]);
        break;
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
                    qDebug() << "[middle]->" << mDevUuid.c_str() << + ":" << QString::number(table, 10) << "," << QString::number(coding) << "," << array[index]["value"].asString().c_str();
    #endif
                }
            }
        }
    }
}

void middleBusiness::zy(Json::Value info)
{
    if(info.isNull())
        return ;    

    if(info.size() >= 1)
    {
        //数据类型
        unsigned char dataType = info[0]["value"].asInt();

        //接收完毕，触发指令

        mapKey vKey(mDevUuid.c_str(), mVirtualTable.c_str(), std::to_string((int)dataType).c_str());

        virtualParams::getInstance()->setValue(vKey);
#if 0
        //总帧数
        unsigned char totleCnt = info[2]["value"].asInt();

        //本帧序号
        unsigned char curCnt = info[3]["value"].asInt();

        if((curCnt + 1) == totleCnt)
        {
            //接收完毕，触发指令
            virtualParams::getInstance()->setValue({mDevUuid, mVirtualTable, std::to_string((int)dataType)});
        }
#endif
    }
}

void middleBusiness::exe4307(const int &coding)
{
    std::string showMsg = "";

    switch(coding)
    {
    case 1:
        showMsg = "总清成功";
        break;
    case 30:
        showMsg = "转入热待发中";
        break;
    case 31:
        showMsg = "热戒备退回冷戒备中";
        break;
    case 32:
        showMsg = "发射退回热戒备";
        break;
    }

    if(showMsg != "")
        toUi(showMsg);
}


void middleBusiness::setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort)
{
    mDstIp = dstIp;
    mDstPort = dstPort;
    mBusObj = obj;
}

void middleBusiness::setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj)
{
    mFrameObj = obj;
}

void middleBusiness::toUi(const std::string &msg, bool state)
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

std::shared_ptr<business> middleBusiness::clone()
{
    middleBusiness *obj = new middleBusiness();

    std::shared_ptr<business> tmp(obj);

    return tmp;
}
