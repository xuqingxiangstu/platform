#ifndef FLOW_H
#define FLOW_H

#include "../runObj/runobj.h"

#include "autorun.h"

#include <memory>

class flow
{
public:
    flow();
    ~flow();
public:
    void setFlowType(std::string type);
    void setMsgObj(Pf::PfAdapter::Adapter *obj);
    void unInit();
    void init(std::string xmlPath);

    void setRunItem(std::string item);
    void getRunItems(std::vector<std::string> &items);
    bool isRunning();
    void runAllFlow();
    void runFlow(std::string);

    //暂停测试
    void suspendTest();
    //继续测试
    void continueTest();

    //停止所有测试
    void exitAllTest();
private:
    void load(std::string);
private:
    bool isLoad;
    std::string proVersion;
    std::vector<std::shared_ptr<runObj>> mObjVec;
    Pf::PfAdapter::Adapter *mMsgObj;
    autoRun *mAutoRun;
};

#endif // FLOW_H
