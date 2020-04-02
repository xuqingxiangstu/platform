#ifndef SUBFLOW_H
#define SUBFLOW_H

#include <vector>
#include <tuple>
#include <string>
#include <memory>
#include <atomic>

#include "../src/PfCommon/dynCreate/dyncreate.h"
#include "../src/PfCommon/TinyXml/tinyxml.h"
#include "../src/PfCommon/jsoncpp/json.h"
#include "../../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"

#include "condition.h"

class action;

#define TESTING "test"
#define TEST_NORMAL "normal"
#define TEST_ERROR  "error"

class subFlow
{
public:
    subFlow(TiXmlElement *);
    ~subFlow();
public:
    void exe();
    void exit(){isStop = true;}
    void setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj);
    void setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter);
    void setRecordUuid(std::string uuid);
    void setFlowUuid(const std::string &uuid){mFLowUuid = uuid;}
    Json::Value getRunItems();
    timing::timingType getTimingType();
private:
    bool perform();
    void init(TiXmlElement *);
    void uiShowMsg(const std::string &msg, bool state = true);
    void uiTestStatus(const std::string &status);
    bool _exe();
private:
    std::shared_ptr<startCondition> mStartCond;
    std::shared_ptr<stopCondition> mStopCond;
    std::shared_ptr<action> mAction;
    std::shared_ptr<timing> mTiming;
    std::atomic_bool isStop;
    std::string mUuid;
    std::string mFLowUuid;
    std::string mDescribe;
    Pf::PfAdapter::Adapter *mUiAdapter;
    std::string mRecordUuid;
};

#endif // SUBFLOW_H
