#include "rcvtask.h"
#include <QTime>
#include <QDateTime>
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

void rcvTask::setRcvUuid(std::tuple<std::string, std::string, std::string, std::string> &uuid)
{
    mAdapters.clear();

    //获取适配器
    //for(auto uuid : uuids)
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
}


void rcvTask::run()
{
    const int rcvMaxSize = 2048;
    char rcvBuf[rcvMaxSize] = {0};
    int rcvSize = 0;
    std::string rcvIp;
    int m1553BCycle = 100;
    unsigned short rcvPort;
    QString recordUuid;
    QString uuid;
    QString ptrl;

    auto beginTime = std::chrono::high_resolution_clock::now();

#ifndef DEBUG_TASK
    while(!mIsStop)
    {
        //遍历接收        
        for(auto adapter : mAdapters)
        {
            auto busObj = std::get<Adapter_Obj_Index>(adapter);

            if(!mPermiss[(qint64)static_cast<void *>(busObj)])
                continue;

            //modify xqx 2020-5-8 17:52:53 1553B查询周期>50ms
            if("m1553Adapter" == busObj->getClassName())
            {
                auto endTime = std::chrono::high_resolution_clock::now();
                auto elapsedTime= std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime);

                if(elapsedTime.count() < m1553BCycle)
                    continue;

                beginTime = std::chrono::high_resolution_clock::now();
            }
            //end

            if(busObj->receiveMsg(rcvBuf, rcvSize, rcvMaxSize, 0))
            {                                
                Json::Value param;

                //modify xqx 2020-6-2 10:52:28 二进制源码转换为十六进制字符串+时间
                QByteArray recordMsg;
                recordMsg += "[";
                recordMsg += QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
                recordMsg += "]";
                recordMsg += "[r]->";
                //end

                //1553B特殊处理 xqx 2020-4-27 19:52:24
                if("m1553Adapter" == busObj->getClassName())
                {
                    unsigned short rt;
                    unsigned short sa;

                    (dynamic_cast<Pf::PfAdapter::m1553Adapter*>(busObj))->getRcvRtAndSa(rt, sa);

                    param["rt"] = rt;
                    param["sa"] = sa;
                }

                //qDebug() << "[rcv]->" << QString(std::get<Adapter_Uuid_Index>(adapter).c_str());

                //step1：解析

                recordUuid = QString(std::get<Record_Uuid_Index>(adapter).c_str());
                uuid = QString(std::get<Adapter_Uuid_Index>(adapter).c_str());
                ptrl = QString(std::get<Adapter_Ptl_Index>(adapter).c_str());

                emit decode(param, recordUuid, uuid, ptrl, QByteArray((char*)rcvBuf, rcvSize), rcvIp.c_str(), rcvPort);

                //modify xqx 2020-6-2 10:52:28 二进制源码转换为十六进制字符串+时间
                recordMsg += QByteArray((char*)rcvBuf, rcvSize).toHex();
                recordMsg += "\n";
                //end

                //step2：存日志
                emit record(QString(std::get<Adapter_Uuid_Index>(adapter).c_str()), recordMsg);
            }
        }

        usleep(1); //让出时间片
    }
#endif
}
