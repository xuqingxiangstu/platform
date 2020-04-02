#include "rcvtask.h"

#include "../src/PfCommon/tools/ut_error.h"

#define DEBUG_TASK  0

rcvTask::rcvTask(QObject *parent):
    QThread(parent),
    mIsStop(false)
{

}

rcvTask::~rcvTask()
{
    if(isRunning())
    {
        mIsStop = true;
#ifndef DEBUG_TASK
        wait();
#endif
    }
}

void rcvTask::startTask()
{
    if(!isRunning())
    {
#ifndef DEBUG_TASK
        start();
#endif
        mIsStop = false;
    }
}

void rcvTask::stopTask()
{
    if(isRunning())
    {
        mIsStop = true;
#ifndef DEBUG_TASK
        wait();
#endif
    }
}

void rcvTask::setRcvUuid(const std::vector<std::string> &uuids)
{
    mAdapters.clear();

    //获取适配器
    for(std::string uuid : uuids)
    {
        Pf::PfAdapter::Adapter *obj;
        if(mPfAdapterManager->getAdapter(uuid, &obj))
        {
            mAdapters.push_back(obj);
        }
        else
        {
            UT_SHOW("[ERROR] not found uuid:" + uuid);
        }
    }
}

void rcvTask::run()
{
    const int rcvMaxSize = 2048;
    char rcvBuf[rcvMaxSize] = {0};
    int rcvSize = 0;
#ifndef DEBUG_TASK
    while(!mIsStop)
    {
        //遍历接收
        for(Pf::PfAdapter::Adapter *adapter : mAdapters)
        {
            if(adapter->receiveMsg(rcvBuf, rcvSize, rcvMaxSize, 0))
            {
                //step1：存日志

                //step2：解析
            }
        }

        usleep(10); //让出时间片
    }
#endif
}
