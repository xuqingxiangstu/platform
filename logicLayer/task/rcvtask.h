#ifndef RCVTASK_H
#define RCVTASK_H

#include <QThread>

#include <atomic>

#include "../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../src/PfAdapter/adapter/Adapter.h"

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
    void setRcvUuid(const std::vector<std::string> &uuids);

    /**
     * @brief setPfAdapterManager   设置适配器管理
     * @param manager   管理句柄
     */
    void setPfAdapterManager(Pf::PfAdapter::PfAdapterManager *manager){mPfAdapterManager = manager;}
private:
    /**
     * @brief qt的run 线程虚函数
     */
    void run();
private:
    std::vector<Pf::PfAdapter::Adapter*> mAdapters;
    Pf::PfAdapter::PfAdapterManager *mPfAdapterManager;

    std::atomic_bool mIsStop;
};

#endif // RCVTASK_H
