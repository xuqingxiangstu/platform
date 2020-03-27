#include "rcvtask.h"

rcvTask::rcvTask(QObject *parent):
    QThread(parent)
{

}

rcvTask::~rcvTask()
{
    if(isRunning())
    {
        quit();
        wait();
    }
}

void rcvTask::startTask()
{

}

void rcvTask::stopTask()
{

}

void rcvTask::setRcvUuid(const std::vector<std::string> &uuids)
{

}

void rcvTask::run()
{

}
