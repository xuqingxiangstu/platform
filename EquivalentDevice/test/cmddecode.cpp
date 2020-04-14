#include "cmddecode.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "../include/PfCommon/tools/ut_error.h"
#include <QDebug>
#include <QMessageBox>


cmdDecode::cmdDecode(QObject *parent)
    :QThread(parent),
      isStop(false),
      mSendObj(nullptr),
      mRcvObj(nullptr)
{

}

cmdDecode::~cmdDecode()
{
    if(isRunning())
    {
        isStop = true;
        //terminate();
        quit();
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

void cmdDecode::run()
{
    if(mRcvObj == nullptr)
        return ;

    const int BUF_SIZE = 102400;
    char buf[BUF_SIZE] = {0};
    int rcvSize = 0;

    while(!isStop)
    {
        if(mRcvObj->receiveMsg(buf, rcvSize, BUF_SIZE, 10))
        {
#ifndef QT_NO_DEBUG
            SHOW(std::string((const char*)buf, rcvSize));
#endif
            QJsonParseError *error=new QJsonParseError;
            QJsonDocument document = QJsonDocument::fromJson(QByteArray((const char*)(buf), rcvSize),error);

            QJsonObject jsonObject = document.object();

            QJsonObject msgObj = jsonObject["msg"].toObject();

            ///消息类型
            QString msgType = jsonObject[MSG_TYPE].toString();

            if(msgType != ""){
                /*if(msgType == "init_flow" && (msgObj["result"].toString().compare("")!=0)&& (msgObj["result"].toString().compare("ok")!=0))
                {
                    emit initFalid(false);
                }else{
                    emit initFalid(true);
                    emit toCmdMsg(jsonObject["msg"].toObject(),msgType);
                }*/
                emit initFalid(true);
                emit toCmdMsg(jsonObject["msg"].toObject(),msgType);
            }else{
                qDebug()<<error->errorString();
            }
            qDebug() << "..";
        }
    }

    qDebug() << "exit rcv";
    isStop = false;
}

