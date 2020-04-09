#include "rcvtask.h"
#include <QTime>
#include "../src/PfCommon/tools/ut_error.h"

//#define DEBUG_TASK  0

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
        initObj();

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
        deleteObj();
    }
}

void rcvTask::setRcvUuid(const std::vector<std::tuple<std::string, std::string, std::string>> &uuids)
{
    mAdapters.clear();

    //获取适配器
    for(auto uuid : uuids)
    {
        Pf::PfAdapter::Adapter *obj;
        if(mPfAdapterManager->getAdapter(std::get<0>(uuid), &obj))
        {
            mAdapters.push_back(std::make_tuple(std::get<0>(uuid), std::get<1>(uuid), obj, std::get<2>(uuid)));
        }
        else
        {
            UT_SHOW("[ERROR] not found uuid:" + std::get<0>(uuid));
        }
    }
}

void rcvTask::deleteObj()
{
    for(auto itor = mRecordsObj.begin(); itor != mRecordsObj.end(); )
    {
        (*itor).reset();

        mRecordsObj.erase(itor);
    }
}

void rcvTask::initObj()
{
    //初始化日志及解析模块

    mRecordsObj.clear();

    for(auto adapter : mAdapters)
    {
         //日志
        std::shared_ptr<Pf::PfCommon::RecordLog> rd = std::make_shared<Pf::PfCommon::RecordLog>();

        rd->setUuid(std::get<Adapter_Uuid_Index>(adapter).c_str());
        rd->setSource(std::get<Adapter_Name_Index>(adapter));

        connect(this, &rcvTask::record, rd.get(), &Pf::PfCommon::RecordLog::record);

        mRecordsObj.emplace_back(rd);

        //解析
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
        for(auto adapter : mAdapters)
        {
            if(std::get<Adapter_Obj_Index>(adapter)->receiveMsg(rcvBuf, rcvSize, rcvMaxSize, 0))
            {
#ifndef DEBUG_TASK
                QString msg = "[" + QDateTime::currentDateTime().toString("hh:mm.ss.zzz") + "]#";
                msg += std::get<Adapter_Name_Index>(adapter).c_str();
                msg += "#(";
                msg += QString::number(rcvSize);
                msg += ")>\n";
                for(int index = 0; index < rcvSize; index++)
                {
                    msg += QString::number(rcvBuf[index] & 0xFF, 16);
                    msg += " ";
                }

                SHOW(msg.toStdString());
#endif
                //step1：解析
                emit decoding(QString(std::get<Adapter_Uuid_Index>(adapter).c_str()), QString(std::get<Adapter_Ptl_Index>(adapter).c_str()), QByteArray((char*)rcvBuf, rcvSize));

                //step2：存日志
                emit record(QString(std::get<Adapter_Uuid_Index>(adapter).c_str()), QByteArray((char*)rcvBuf, rcvSize));
            }
        }

        usleep(10); //让出时间片
    }
#endif
}
