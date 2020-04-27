#ifndef CMDDECODE_H
#define CMDDECODE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <map>
#include <QMap>
#include <string>
#include "../src/PfAdapter/adapter/Adapter.h"
#include "../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../src/PfBus/zeroMq/zmqsend.h"
#include "../src/PfCommon/jsoncpp/json.h"
#include "./flow/flow.h"
#include "./task/rcvtask.h"
#define MSG_TYPE    "msgType"

#define SINGLE_SEND_TEST    "single_send_test"

#define INIT_FLOW       "init_flow"
#define INIT_TEST       "init"
#define MANUALTRIGGER   "manualTrigger"
#define START_TEST      "start"
#define STOP_TEST       "stop"
#define SUSPEND_TEST    "suspend"
#define EXIT_TEST       "exit"
#define CONTINUE_TEST   "continue"
#define GET_RUNITES     "runitems"
#define LOAD            "load"

#define OK      "ok"
#define RUNNING "running"
#define NOT_RUNNING "not running"
#define NO_FLOW "no flow"

#define QUERY_ID "sa_query"

#define NORMAL_FLOW "normal"
#define FAULT_FLOW  "fault"

#define DEBUG_SHOW  0

//#undef QT_NO_DEBUG

#undef QT_NO_DEBUG

using namespace Pf;

class cmdDecode : public QObject
{
    Q_OBJECT
public:
    cmdDecode(PfAdapter::Adapter  *obj, QObject *parent = 0);
    ~cmdDecode();
public:
    /**
     * @brief initPrograme  初始化应用程序
     */
    void initPrograme();
#ifndef QT_NO_DEBUG
public:
#else
private:
#endif
    /**
     * @brief cmdMsg    命令消息
     * @param jsObj     Json对象
     */
    void cmdMsg(Json::Value jsObj);

    /**
     * @brief switchOutMsg  开关量输出消息
     * @param jsObj         对象
     */
    void switchOutMsg(Json::Value jsObj);

    /**
     * @brief analogOutMsg  模拟量输出消息
     * @param jsObj         对象
     */
    void analogOutMsg(Json::Value jsObj);


    /**
     * @brief analogOutMsg  串口输出消息
     * @param jsObj         对象
     */
    void rs422OutMsg(Json::Value jsObj);

    /**
     * @brief respond   命令应答
     * @param code      JSON文件
     */
    void respond(const std::string &code);

    /**
     * @brief load  加载流程配置
     * @param flowType  流程类型（normal：正常、fault：故障)
     * @param path  流程配置文件路径
     */
    void load(std::string flowType, std::string path);

    void initFlow(const Json::Value &msg);

    /**
     * @brief startTest 开始测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     * @param items     测试项
     */
    void startTest(const Json::Value &msg);

    void manualTrigger(const Json::Value &msg);

    /**
     * @brief stopTest  停止测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void stopTest(const Json::Value &);

    /**
     * @brief suspendTest   暂停测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void suspendTest(std::string flowType);

    /**
     * @brief exitTest  退出测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void exitTest(std::string flowType);

    /**
     * @brief continueTest  继续测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void continueTest(std::string flowType);

    /**
     * @brief getRunItems   获取待测试Items
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void getRunItems();

    std::string resultMsg(std::string type, const std::string &msg, std::string uuid = "");

    bool resetAdapter();

    void getDevInfo(const std::string &sys_uuid, Json::Value &info);
    void getDestDevIpPort(const std::string &uuid, Json::Value &info);
    void singleSendTest(Json::Value value);
signals:

    /**
     * @brief setAnalogOut  模拟量输出数据
     * @param cardId        板卡ID
     * @param channel       通道号
     * @param min           最小值
     * @param max           最大值
     */
    void setAnalogOut(std::string cardId, int channel, double min, double max);

    void singleTestSig(Json::Value value);
public slots:

    /**
     * @brief upDateAnalogInData    更新模拟量数据
     * @param id                    设备ID
     * @param data                  模拟量数据
     * @param len                   数据长度
     */
    //void upDateAnalogInData(QString id, double *data, int len);

    /**
     * @brief parse     命令解析
     * @param srcJson   JSON文件源
     */
    void parse(QString srcJson);
private:
    PfAdapter::Adapter *mCmdSendObj;  ///< 命令发送对象
    //PfAdapter::Adapter *mAnalogSendObj;   ///< 模拟量输入发送对象
    std::shared_ptr<PfAdapter::PfAdapterManager> mAdpterManagerObj; ///<硬件设备管理对象
    std::shared_ptr<PfIcdWorkBench::icdFrameAdapter> mIcdFrameAdpter; ///<组帧、解帧协议适配对象
    std::atomic_bool mIsInitSuccessful;     ///<初始化是否成功
    QMap<std::string, bool> mIsInitFlow;           ///<流程是否初始化
    std::map<std::string, std::shared_ptr<flowManager>> mFLowsObj;    ///< 流程句柄
    std::shared_ptr<rcvTask> mRcvMsgTask;      ///< 接收消息任务  
};

#endif // CMDDECODE_H
