#ifndef ACTION_H
#define ACTION_H

#include "../src/PfCommon/TinyXml/tinyxml.h"
#include "../../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../../src/PfCommon/jsoncpp/json.h"

#include <memory>

class frame;

class action
{   
public:
    action();
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
