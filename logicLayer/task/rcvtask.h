#ifndef RCVTASK_H
#define RCVTASK_H

#include <QThread>

#include <atomic>

#include "../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../src/PfAdapter/adapter/Adapter.h"
#include "../src/PfCommon/recordLog/recordlog.h"

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
    void setRcvUuid(const std::vector<std::tuple<std::string, std::string, std::string>> &uuids);

    /**
     * @brief setPfAdapterManager   设置适配器管理
     * @param manager   管理句柄
     */
    void setPfAdapterManager(Pf::PfAdapter::PfAdapterManager *manager){mPfAdapterManager = manager;}
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
     */
    void decoding(QString uuid, QString protocol, QByteArray msg);
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
        Adapter_Ptl_Index
    };
    std::vector<std::tuple<std::string, std::string, Pf::PfAdapter::Adapter*, std::string>> mAdapters;
    Pf::PfAdapter::PfAdapterManager *mPfAdapterManager;
    std::vector<std::shared_ptr<Pf::PfCommon::RecordLog>> mRecordsObj; //日志记录
    std::atomic_bool mIsStop;
};

#endif // RCVTASK_H
