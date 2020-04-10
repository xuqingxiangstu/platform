#include "cmddecode.h"

#include <QDebug>
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
#ifndef DEBUG_DECODE
    std::string sendIp = "127.0.0.1";
    std::string sendPort = "2203";

    std::string rcvIp = "";
    std::string rcvPort = "2204";

    mSendObj = new PfAdapter::ZmqSendAdapter();
    dynamic_cast<PfAdapter::ZmqSendAdapter*>(mSendObj)->init(sendIp, sendPort);

    mRcvObj = new PfAdapter::ZmqRcvAdapter();
    dynamic_cast<PfAdapter::ZmqRcvAdapter*>(mRcvObj)->init(rcvIp, rcvPort);
#endif
}

cmdDecode::~cmdDecode()
{
#ifndef DEBUG_DECODE
    if(isRunning())
    {
        isStop = true;
        terminate();
        wait();
        isStop = false;
    }
#endif
}

void cmdDecode::startDecode()
{
#ifndef DEBUG_DECODE
    if(!isRunning())
    {
        isStop = false;
        start();
    }
#endif
}


void cmdDecode::onSendCmd(QString json)
{
#ifndef DEBUG_DECODE
    if(mSendObj)
        mSendObj->sendMsg(json.toStdString().c_str(), json.toStdString().size());
#endif
}

void cmdDecode::onSendJson(Json::Value jsonObj)
{
#ifndef DEBUG_DECODE
    //加协议
    if(mSendObj)
    {
        Json::Value sendJs;
        sendJs["msgType"] = "single_send_test";

        sendJs["msg"] = jsonObj;

        //qDebug() << "[" << sendJs.toStyledString().size() << "]" << sendJs.toStyledString().c_str();

        mSendObj->sendMsg(sendJs.toStyledString().c_str(), sendJs.toStyledString().size());
    }
#endif
}

void cmdDecode::run()
{
#ifndef DEBUG_DECODE
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
#endif
}

