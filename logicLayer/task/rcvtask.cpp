#include "rcvtask.h"
#include <QTime>
#include "../src/PfCommon/tools/ut_error.h"
#include "../src/PfAdapter/m1553Adapter/m1553adapter.h"
//#define DEBUG_TASK  0

#include <QDebug>

rcvTask::rcvTask(QObject *parent):
    QThread(parent),
    mIsStop(false),
    mDecodingPoolObj(nullptr)
{

}

void rcvTask::setIcdFrameAdpter(std::shared_ptr<Pf::PfIcdWorkBench::icdFrameAdapter> obj)
{
    mIcdFrameManager = obj;
}

void rcvTask::setPfAdapterManager(std::shared_ptr<Pf::PfAdapter::PfAdapterManager> manager)
{
    mPfAdapterManager = manager;
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

void rcvTask::onSwitchPermiss(qint64 obj, bool status)
{
    mPermiss[obj] = status;
}

void rcvTask::setRcvUuid(const std::vector<std::tuple<std::string, std::string, std::string, std::string>> &uuids)
{
    mAdapters.clear();

    //获取适配器
    for(auto uuid : uuids)
    {
        Pf::PfAdapter::Adapter *obj;
        if(mPfAdapterManager->getAdapter(std::get<0>(uuid), &obj))
        {
            mPermiss[(qint64)static_cast<void *>(obj)] = true;
            mAdapters.push_back(std::make_tuple(std::get<0>(uuid), std::get<1>(uuid), obj, std::get<2>(uuid), std::get<3>(uuid)));
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
    if(mDecodingPoolObj.get() == nullptr)
    {
        mDecodingPoolObj = std::make_shared<decodingPool>();

        mDecodingPoolObj->setIcdFrameAdpter(mIcdFrameManager);
        mDecodingPoolObj->setPfAdapterManager(mPfAdapterManager);

        connect(this, &rcvTask::decode, mDecodingPoolObj.get(), &decodingPool::decode);
        connect(mDecodingPoolObj.get(), &decodingPool::result, this, &rcvTask::decodeResult);
    }

    //初始化日志模块

    mRecordsObj.clear();

    //日志
    for(auto adapter : mAdapters)
    {         
        std::shared_ptr<Pf::PfCommon::RecordLog> rd = std::make_shared<Pf::PfCommon::RecordLog>();

        rd->setUuid(std::get<Adapter_Uuid_Index>(adapter).c_str());
        rd->setSource(std::get<Adapter_Name_Index>(adapter));

        connect(this, &rcvTask::record, rd.get(), &Pf::PfCommon::RecordLog::record);

        mRecordsObj.emplace_back(rd);        
    }
}


void rcvTask::run()
{
    const int rcvMaxSize = 2048;
    char rcvBuf[rcvMaxSize] = {0};
    int rcvSize = 0;
    std::string rcvIp;
    unsigned short rcvPort;

#ifndef DEBUG_TASK
    while(!mIsStop)
    {
        //遍历接收        
        for(auto adapter : mAdapters)
        {
            auto busObj = std::get<Adapter_Obj_Index>(adapter);

            if(!mPermiss[(qint64)static_cast<void *>(busObj)])
                continue;

            if(busObj->receiveMsg(rcvBuf, rcvSize, rcvMaxSize, 0))
            {                                
                Json::Value param;

                //1553B特殊处理 xqx 2020-4-27 19:52:24
                if("m1553Adapter" == busObj->getClassName())
                {
                    unsigned short rt;
                    unsigned short sa;

                    (dynamic_cast<Pf::PfAdapter::m1553Adapter*>(busObj))->getRcvRtAndSa(rt, sa);

                    param["rt"] = rt;
                    param["sa"] = sa;
                }

                //step1：解析                
                emit decode(param, QString(std::get<Record_Uuid_Index>(adapter).c_str()), QString(std::get<Adapter_Uuid_Index>(adapter).c_str()), QString(std::get<Adapter_Ptl_Index>(adapter).c_str()), QByteArray((char*)rcvBuf, rcvSize), rcvIp.c_str(), rcvPort);

                //step2：存日志
                emit record(QString(std::get<Adapter_Uuid_Index>(adapter).c_str()), QByteArray((char*)rcvBuf, rcvSize));
            }
        }

        usleep(10); //让出时间片
    }
#endif
}
