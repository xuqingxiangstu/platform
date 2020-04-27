#ifndef RCVTASK_H
#define RCVTASK_H

#include <QThread>

#include <atomic>

#include "../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../src/PfAdapter/adapter/Adapter.h"
#include "../src/PfCommon/recordLog/recordlog.h"
#include "../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../decoding/decoding.h"

//#define DEBUG_TASK 0

class rcvTask : public QThread
{
    Q_OBJECT
public:
    rcvTask(QObject *parent = nullptr);
    ~rcvTask();
public:
    void startTask();
    void stopTask();
    /**
     * @brief setRcvUuid    设置接收uuids
     * @param uuids
     */
    void setRcvUuid(const std::vector<std::tuple<std::string, std::string, std::string,std::string>> &uuids);

    /**
     * @brief setPfAdapterManager   设置适配器管理
     * @param manager   管理句柄
     */
    void setPfAdapterManager(std::shared_ptr<Pf::PfAdapter::PfAdapterManager> manager);

    /**
     * @brief setIcdFrameAdpter 设置ICD管理句柄
     * @param obj   句柄
     */
    void setIcdFrameAdpter(std::shared_ptr<Pf::PfIcdWorkBench::icdFrameAdapter> obj);
signals:
    /**
     * @brief record    数据存储
     * @param uuid      UUID
     * @param Msg       消息
     */
    void record(QString uuid, QByteArray Msg);

    /**
     * @brief decoding  解码
     * @param uuid      UUID
     * @param protocol  协议
     * @param msg       消息
     * @param rcvIp     接收IP地址
     * @param rcvPort   接收端口号
     */
    void decode(Json::Value param, QString recordUuid, QString uuid, QString protocol, QByteArray msg, QString rcvIp, int rcvPort);

    /**
     * @brief decodeResult  解码结果
     * @param v 结果
     */
    void decodeResult(Json::Value v);
public slots:
    /**
     * @brief onSwitchPermiss   切换接收权限
     */
    void onSwitchPermiss(qint64, bool);
private:
    /**
     * @brief qt的run 线程虚函数
     */
    void run();

    void initObj();

    void deleteObj();
private:
    enum{
        Adapter_Uuid_Index,
        Adapter_Name_Index,
        Adapter_Obj_Index,
        Adapter_Ptl_Index,
        Record_Uuid_Index
    };
    std::vector<std::tuple<std::string, std::string, Pf::PfAdapter::Adapter*, std::string,std::string>> mAdapters;
    std::shared_ptr<Pf::PfAdapter::PfAdapterManager> mPfAdapterManager;
    std::shared_ptr<Pf::PfIcdWorkBench::icdFrameAdapter> mIcdFrameManager;
    std::vector<std::shared_ptr<Pf::PfCommon::RecordLog>> mRecordsObj; //日志记录
    std::shared_ptr<decodingPool> mDecodingPoolObj;    //解码    
    std::atomic_bool mIsStop;

    QMap<qint64, bool> mPermiss;
};

#endif // RCVTASK_H
