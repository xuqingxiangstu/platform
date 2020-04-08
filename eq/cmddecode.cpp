#include "cmddecode.h"


#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "../src/PfCommon/tools/ut_error.h"
#include "../src/PfCommon/jsoncpp/json.h"
#include "../src/PfAdapter/ZmqRcvAdapter/ZmqRcvAdapter.h"
#include "../src/PfAdapter/ZmqSendAdapter/ZmqSendAdapter.h"

using namespace Pf;

cmdDecode::cmdDecode(QObject *parent)
    :QThread(parent),
      isStop(false),
      mSendObj(nullptr),
      mRcvObj(nullptr)
{
    std::string sendIp = "127.0.0.1";
    std::string sendPort = "2202";

    std::string rcvIp = "";
    std::string rcvPort = "2204";

    mSendObj = new PfAdapter::ZmqSendAdapter();
    dynamic_cast<PfAdapter::ZmqSendAdapter*>(mSendObj)->init(sendIp, sendPort);

    mRcvObj = new PfAdapter::ZmqRcvAdapter();
    dynamic_cast<PfAdapter::ZmqRcvAdapter*>(mRcvObj)->init(rcvIp, rcvPort);
}

cmdDecode::~cmdDecode()
{
    if(isRunning())
    {
        isStop = true;
        terminate();
        wait();
        isStop = false;
    }
}

void cmdDecode::startDecode()
{
    if(!isRunning())
    {
        isStop = false;
        start();
    }
}


void cmdDecode::onSendCmd(QString json)
{
    if(mSendObj)
        mSendObj->sendMsg(json.toStdString().c_str(), json.toStdString().size());
}

void cmdDecode::onSendJson(Json::Value jsonObj)
{
    //加协议
    if(mSendObj)
    {
        Json::Value sendJs;
        sendJs["msgType"] = "single_send_test";

        sendJs["msg"] = jsonObj;

        mSendObj->sendMsg(sendJs.toStyledString().c_str(), sendJs.toStyledString().size());
    }
}

void cmdDecode::run()
{
    if(mRcvObj == nullptr)
        return ;

    const int BUF_SIZE = 10240;
    char buf[BUF_SIZE] = {0};
    int rcvSize = 0;

    while(!isStop)
    {
        if(mRcvObj->receiveMsg(buf, rcvSize, BUF_SIZE))
        {
#ifndef QT_NO_DEBUG
            SHOW(std::string((const char*)buf, rcvSize));
#endif
            Json::Value root;
            Json::Reader reader;
            if(reader.parse(std::string((const char*)buf, rcvSize),root))
            {
                std::string msgType = root[MSG_TYPE].asString();
                if(msgType == "single_send_test")
                {
                    emit testMsg(root["msg"]);
                }
            }

        }
    }
}

