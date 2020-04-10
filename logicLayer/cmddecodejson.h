#ifndef CMDDECODE_H
#define CMDDECODE_H

#include <QObject>
#include <QMap>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "../src/PfAdapter/adapter/Adapter.h"
#include "../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../src/PfFlow/RunFlow/runflow.h"
#define MSG_TYPE    "msgType"
#define CMD_TYPE    "cmd"
#define ANALOG_TYPE "analog_out"
#define RS422_TYPE  "RS422"
#define SWITCH_TYPE "switch_out"
#define CMD_MSG         "msg"      // fjt 更改，与系统冲突
#define TYPE        "type"

#define INIT_TEST       "init"
#define START_TEST      "start"
#define STOP_TEST       "stop"
#define SUSPEND_TEST    "suspend"
#define EXIT_TEST       "exit"
#define CONTINUE_TEST   "continue"
#define GET_RUNITES     "get_runitems"
#define LOAD            "load"

#define OK      "ok"
#define RUNNING "running"
#define NO_FLOW "no flow"

#define QUERY_ID "sa_query"

#define NORMAL_FLOW "normal"
#define FAULT_FLOW  "fault"
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
    void cmdMsg(QJsonObject jsObj);

    /**
     * @brief switchOutMsg  开关量输出消息
     * @param jsObj         对象
     */
    void switchOutMsg(QJsonArray jsObj);

    /**
     * @brief analogOutMsg  模拟量输出消息
     * @param jsObj         对象
     */
    void analogOutMsg(QJsonArray jsObj);


    /**
     * @brief analogOutMsg  串口输出消息
     * @param jsObj         对象
     */
    void rs422OutMsg(QJsonObject jsObj);

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
    void load(QString flowType, QString path);

    /**
     * @brief startTest 开始测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     * @param items     测试项
     */
    void startTest(QString flowType, QString items);

    /**
     * @brief stopTest  停止测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void stopTest(QString flowType);

    /**
     * @brief suspendTest   暂停测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void suspendTest(QString flowType);

    /**
     * @brief exitTest  退出测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void exitTest(QString flowType);

    /**
     * @brief continueTest  继续测试
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void continueTest(QString flowType);

    /**
     * @brief getRunItems   获取待测试Items
     * @param flowType  流程类型（normal：正常、fault：故障)
     */
    void getRunItems(QString flowType);
signals:

    /**
     * @brief setAnalogOut  模拟量输出数据
     * @param cardId        板卡ID
     * @param channel       通道号
     * @param min           最小值
     * @param max           最大值
     */
    void setAnalogOut(QString cardId, int channel, double min, double max);
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
    PfAdapter::Adapter *mAnalogSendObj;   ///< 模拟量输入发送对象
    PfAdapter::PfAdapterManager *mAdpterManagerObj; ///<硬件设备管理对象
  //  switchQueryTask *mSwitchQueryTask;      ///< 开关量输入发送对象
  //  std::vector<analogInTask*> mAnalogInTasks;  ///< 模拟量采集
   // std::vector<analogOutTask*> mAnalogOutTasks;///< 模拟量输出
  //  updateAnalogTask *mUpdateAnalogTask; ///< 模拟量数据更新任务
    QMap<QString, RunFlow*> mFLows;    ///< 流程句柄
};

#endif // CMDDECODE_H
