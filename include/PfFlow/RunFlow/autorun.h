#ifndef AUTORUN_H
#define AUTORUN_H

#include "../runObj/runobj.h"

#include <vector>
#include <string>
#include <tuple>
#include <atomic>
#include <memory>


class autoRun : public runObj
{
    enum{
        KeyIndex = 0,
        ObjIndex
    };
    using ValueType = std::tuple<std::string, runObj*>;

public:
    ~autoRun();
    autoRun();
public:
    void setFlowType(std::string type);
    void setMsgObj(Pf::PfAdapter::Adapter *obj);
    void init(TiXmlElement *);
    void unInit();

    void getRunItems(std::vector<std::string> &items);
    bool isRunning(){return mIsRunning;}
    void runAllFlow();
    void runFlow(const std::string &);

    //暂停测试
    void suspendTest();
    //继续测试
    void continueTest();

    //停止所有测试
    void exitAllTest();

    /**
     * @brief getObj        通过索引获取步骤句柄
     * @param proceIndex
     * @param testCaseIndex
     * @param stepIndex
     * @return
     */
    runObj *getObj(std::string proceIndex, std::string testCaseIndex, std::string stepIndex);
private:
    void updateMsg(const std::string &);
private:
    Pf::PfAdapter::Adapter *mMsgObj;
private: 
    std::vector<ValueType> mRunVec;
    std::string mCurFlowType;
    std::atomic_int mCurItemIndex;
    std::atomic_bool mIsRunning;
    std::atomic_bool mIsExit;

    Pf::PfAdapter::Adapter *mObserverObj;
};

#endif // AUTORUN_H
