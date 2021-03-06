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

//#define LH_TEST 0     //老化测试开关

#define USE_STD_THRED   0   //1:c++，0:qt

class flowManager;

class flow : public QObject
{
    Q_OBJECT

    enum
    {
        Uuid_Index,
        SubFlow_Index
    };
public:
    flow(TiXmlElement *, QObject *parent = 0);
    void setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj);
    void setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter);
    void setFlowUuid(const std::string uuid){mUuid = uuid;}
    void setRecordUuid(std::string uuid);
    Json::Value getRunItems();
    void enforceExe(std::string subFlowUuid);
    void exe(flowManager *manager, std::string subFlowUuid);
    void exit();
private:
    void restoreInitStateCmd();
private:
    std::vector<std::tuple<std::string, std::shared_ptr<subFlow>>> mSubFlowObjs;
    flowManager *mFlowManagerObj;
    std::atomic_bool isStop;
    Pf::PfAdapter::Adapter *mUiAdapter;
    std::string mRecordUuid;
    std::string mUuid;
};

class flowManager : public QObject
{
    Q_OBJECT
    enum
    {
        Uuid_Index,
        Des_Index,
        Flow_Index
    };
public:
    flowManager(QObject *parent = 0);
    ~flowManager();
    void init(std::string path);
    void setAdapterManager(Pf::PfAdapter::PfAdapterManager *adapterManagerObj);
    void setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter);
    Json::Value getRunItems();
    bool isRunning(){return mIsRunning;}
    void setRecordUuid(std::string uuid);
    //void exe();
    void enforceExe(std::string flowUuid, std::string subFlowUuid);
    void exe(std::string flowUuid, std::string subFlowUuid);
    void exit();    
    void exeOver(bool status, std::string info);
    std::string eqSystemUuid(){return mEqSystemUuid;}
    std::string dstSystemUuid(){return mDstSystemUuid;}
    std::string eqProtocol(){return mEqProtocol;}
private:
    std::vector<std::tuple<std::string, std::string, std::shared_ptr<flow>>> mFlowsObj;
    std::atomic_bool mIsRunning;
    Pf::PfAdapter::Adapter *mUiAdapter;
    std::string mRecordUuid;
    std::string mEqSystemUuid;  //等效系统UUID
    std::string mDstSystemUuid;  //目标系统UUID
    std::string mEqProtocol;    //等效系统协议
    std::string mAskFlag;       //应答标志
    std::vector<std::shared_ptr<QThread>> mExeThreads;
    //std::vector<std::thread> mRunThreadObj;
};

#endif // FLOW_H
