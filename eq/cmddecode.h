#ifndef CMDDECODE_H
#define CMDDECODE_H

#include <QThread>
#include <string>
#include <atomic>
#include <QJsonObject>
#include "../src/PfCommon/jsoncpp/json.h"
#include "../src/PfAdapter/adapter/Adapter.h"
#include <memory>

#define MSG_TYPE    "msgType"


class cmdDecode : public QThread
{
    Q_OBJECT
public:
    cmdDecode(QObject *parent = 0);
    ~cmdDecode();
public:
    void startDecode();
private:
    void run();
signals:    
    void testMsg(Json::Value msg);
    void showErrMsg(QString msg);
public slots:
    void onSendCmd(QString json);
    void onSendJson(Json::Value jsonObj);
public:
    std::atomic_bool isStop;
    Pf::PfAdapter::Adapter *mRcvObj;
    Pf::PfAdapter::Adapter *mSendObj;
};

#endif // CMDDECODE_H
