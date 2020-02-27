#ifndef RUNPROCEDURE_H
#define RUNPROCEDURE_H

#include "../runObj/runobj.h"
#include "runcase.h"

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <unordered_map>

#define USE_MAP 0


class TestProcedure : public runObj
{
    enum
    {
        KeyIndex = 0,
        ObjIndex
    };
public:
    TestProcedure();
    ~TestProcedure();
public:
    void init(TiXmlElement *);
    void unInit();

    bool exec();

    void notify(TestCase*); //若状态变化，则遍历观察者，逐个通知更新
    void setFlowType(std::string type);
    void setAutoRunObj(runObj *obj);
    bool getResult(){ return mTestResult; }
    std::string getDescribe(){ return strDescribe; }
    void getRunItems(std::vector<std::string> &);

    //设置父节点id
    void setFatherId(std::string id){ strFatherId = id; }

    //暂停测试
    void suspendTest();
    //继续测试
    void continueTest();
    //停止测试
    void exitTest();

    void setMsgObj(Pf::PfAdapter::Adapter *obj);
    runObj *getObj(std::string testCaseIndex, std::string stepIndex);

    void setAdapter(Pf::PfAdapter::PfAdapterManager *adapter) override;
    void setIcdframeadapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter) override;
private:
    void run();
    void updateMsg(const std::string &);
private:
    Pf::PfAdapter::Adapter *mMsgObj;
    using ValueType = std::tuple<std::string, TestCase*>;
    std::vector<ValueType> mTestCaseVec;
    std::string mCurFlowType;
    std::atomic_bool isSuspend;
    std::atomic_bool isTesting;
    std::atomic_bool isExit;
    std::atomic_bool isRuning;
    int mCurTestIndex;
    std::string strDescribe;
    bool mTestResult;
    std::string strFatherId;
    std::mutex mMutex;
    std::condition_variable mCv;
};

#endif // RUNPROCEDURE_H
