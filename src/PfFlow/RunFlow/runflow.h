#ifndef RUNFLOW_H
#define RUNFLOW_H
/**
* @file RunFlow.h
* @file autor xqx
* @biref 执行配置的测试流程flow
* @version 0.1
* @data 2020-2-11
* @Copyright Copyright (c) 2020
* @todo 增加与界面信息交互的相关函数
*/
#include "runflow_global.h"
#include "../runObj/runobj.h"
#include "autorun.h"

#include <memory>
/**
 * @brief The RunFlow class
 * 运行xml流程
 */
class RUNFLOWSHARED_EXPORT RunFlow
{
public:
    /**
    * @brief RunFlow 构造函数
    * 初始化相关参数
    */
    RunFlow();
    ~RunFlow();
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

    ///暂停测试
    void suspendTest();
    ///继续测试
    void continueTest();

    ///停止所有测试
    void exitAllTest();

    void setAdapter(Pf::PfAdapter::PfAdapterManager *adapter);
    void setIcdframeadapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter);
private:
    void load(std::string);
private:
    bool isLoad;
    std::string proVersion;
    std::vector<std::shared_ptr<runObj>> mObjVec;
    Pf::PfAdapter::Adapter *mMsgObj;///消息传输适配对象
    autoRun *mAutoRun;///自动测试控制实例
};

#endif // RUNFLOW_H
