#ifndef RCVTASK_H
#define RCVTASK_H

#include <QThread>

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
    void setRcvUuid(const std::vector<std::string> &uuids);
private:
    /**
     * @brief qt的run 线程虚函数
     */
    void run();
private:
    std::vector<Pf::PfAdapter::Adapter*> mAdapters;
};

#endif // RCVTASK_H
