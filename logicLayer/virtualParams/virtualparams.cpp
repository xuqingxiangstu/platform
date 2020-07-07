#include "virtualparams.h"
#include <thread>


virtualParams *virtualParams::mInstance = nullptr;
std::mutex virtualParams::mInMutex;

QMutex virtualParams::mParamsUpDateMutex;
QWaitCondition virtualParams::mParamsUpdateCondition;

virtualParams::virtualParams()
{
    //初始化参数存活时间
    activeTime::getInstance();
#if 0
    setValue({"12", "1103", "2"}, Json::Value(3.4));
    setValue({"34", "4401", "02"}, Json::Value(4));

    bool res = false;
    res = isMeet({"12", "1103", "2"}, Json::Value(3.3));
    res = isMeet({"12", "1103", "2"}, Json::Value(3.4));
    res = isMeet({"34", "4401", "02"}, Json::Value(4));
    res = isMeet({"34", "4401", "02"}, Json::Value(3));
    res = isMeet({"34", "4401", "12"}, Json::Value(4));
    int i = 0;
    i++;
#endif
}

void virtualParams::setValue(const keyType &key, Json::Value value)
{
#if USE_QT_LOCK
    mDataMutex.lockForWrite();
#else
    mDataMutex.lock();
#endif

    QString zcValue = key.second + "_" + key.three;

    if(mHashParamsManager.contains(QPair<QString, QString>(key.first, zcValue)))
    {
        mHashParamsManager[QPair<QString, QString>(key.first, zcValue)]->setValue(value);
    }
    else
    {
        mHashParamsManager[QPair<QString, QString>(key.first, zcValue)] = new valueType(value);
    }

    //增加时间戳
    mHashParamsManager[QPair<QString, QString>(key.first, zcValue)]->setStartTime(std::chrono::high_resolution_clock::now());

    mDataMutex.unlock();

#if USE_BLOCK_MODE
    //唤醒其他线程
    mParamsUpdateCondition.wakeAll();
#endif
}

bool virtualParams::isMeet(const keyType &key, Json::Value value)
{
    bool res = false;

#if USE_QT_LOCK
    mDataMutex.lockForRead();
#else
    mDataMutex.lock();
#endif

    auto pair = QPair<QString, QString>(key.first, key.second + "_" + key.three);

    //符合条件后设置为无效
    if(mHashParamsManager.contains(pair))
    {
        res = mHashParamsManager[pair]->isMeet(value);
        //mHashParamsManager[QPair<QString, QString>(key.first.c_str(), zcValue)]->setInValid();
    }

    mDataMutex.unlock();

    return res;
}
