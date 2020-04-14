#ifndef CMDDECODE_H
#define CMDDECODE_H

#include <QThread>
#include <string>
#include <atomic>
#include <QJsonObject>

#include "../include/PfAdapter/adapter/Adapter.h"
#include <memory>

#define MSG_TYPE    "msgType"
//#define CMD_TYPE    "runitems"
//#define MSG         "msg"

class cmdDecode : public QThread
{
    Q_OBJECT
public:
    cmdDecode(QObject *parent = 0);
    ~cmdDecode();
public:
    void startDecode();
    void setRcvObj(Pf::PfAdapter::Adapter *obj){mRcvObj = obj;}
    void setSendObj(Pf::PfAdapter::Adapter *obj){mSendObj = obj;}
private:
    void run();
signals:
    void toCmdMsg(QJsonObject cmdObj,QString msgType);
    //void showErrMsg(QString msg);
    void initFalid(bool);
public slots:
    void onSendCmd(QString json);
public:
    std::atomic_bool isStop;
    Pf::PfAdapter::Adapter *mRcvObj;
    Pf::PfAdapter::Adapter *mSendObj;
};

#endif // CMDDECODE_H
