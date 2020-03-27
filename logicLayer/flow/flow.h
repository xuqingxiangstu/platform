#ifndef FLOW_H
#define FLOW_H

#include <string>
#include <thread>

#include "subflow.h"
#include "../../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../../src/PfCommon/jsoncpp/json.h"
#include <vector>
#include <tuple>

class flowManager;

class flow
{
    enum
    {
        Uuid_Index,
        SubFlow_Index
    };
public:
    flow(TiXmlElement *);
    void setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj);
    void setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter);
    void setRecordUuid(std::string uuid);
    Json::Value getRunItems();
    void exe(flowManager *manager, std::string subFlowUuid);
    void exit();
private:
    std::vector<std::tuple<std::string, std::shared_ptr<subFlow>>> mSubFlowObjs;
    flowManager *mFlowManagerObj;
    std::string mRecordUuid;
};

class flowManager
{
    enum
    {
        Uuid_Index,
        Des_Index,
        Flow_Index
    };
public:
    flowManager();
    ~flowManager();
    void init(std::string path);
    void setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj);
    void setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter);
    Json::Value getRunItems();
    bool isRunning(){return mIsRunning;}
    void setRecordUuid(std::string uuid);
    //void exe();
    void exe(std::string flowUuid, std::string subFlowUuid);
    void exit();    
    void exeOver();
private:
    std::vector<std::tuple<std::string, std::string, std::shared_ptr<flow>>> mFlowsObj;
    std::atomic_bool mIsRunning;
    std::string mRecordUuid;

    //std::vector<std::thread> mRunThreadObj;
};

#endif // FLOW_H
