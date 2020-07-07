#ifndef ACTION_H
#define ACTION_H

#include "../src/PfCommon/TinyXml/tinyxml.h"
#include "../../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../../src/PfCommon/jsoncpp/json.h"

#include <memory>
#include <QByteArray>
#include <QString>
#include <QObject>

class frame;

class action : public QObject
{   
    Q_OBJECT
public:
    action(QObject *parent = 0);
    ~action();
public:
    void init(TiXmlElement *);
    bool isConform();
    bool exe();
    void setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj);
    void setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter);
    void setRecordUuid(std::string uuid);
    Json::Value getRunItems();
    static std::string className(){return "action";}
private:
    void toUi(const std::string &msg, bool state = true);
    void toLog(const char *msg, const int &msgSize);
signals:
    void record(QString uuid, QByteArray Msg);
private:
    std::string mDstSysUuid;
    unsigned char mRcvBus[2048];
    int mTimeOut;
    std::shared_ptr<frame> mFrameObj;
    Pf::PfAdapter::PfAdapterManager *mAdapterManagerObj;
    Pf::PfAdapter::Adapter *mUiAdapter;
    std::string mRecordUuid;
};



#endif // ACTION_H
