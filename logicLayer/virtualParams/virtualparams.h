#ifndef VIRTUALPARAMS_H
#define VIRTUALPARAMS_H

/**
 * @brief The virtualParams class 虚拟参数
 */

#include <unordered_map>
#include <mutex>
#include <string>
#include <chrono>
#include <QHash>
#include <QMutex>
#include <QDebug>
#include <QWaitCondition>
#include <QReadWriteLock>
#include "activetime.h"
#include "../src/PfCommon/jsoncpp/json.h"

#define USE_BLOCK_MODE  1   // 0:非阻塞、1:阻塞

#define USE_QT_LOCK 1       //1:QT 读写锁，0：c++锁

class mapKey
{
public:
    mapKey(){}
    ~mapKey(){
    }
    mapKey(QString f, QString s, QString t):first(f), second(s), three(t){}
public:
    QString first;
    QString second;
    QString three;
};

class mapValue
{
public:   
    mapValue(Json::Value value = Json::Value()):isValid(true),mCurValue(value){}

    void setStartTime(std::chrono::system_clock::time_point time){startTime = time;}

    bool isMeet(Json::Value value = Json::Value())
    {
        //modify xqx 20200429 增加时间判断
        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime= std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        if(elapsedTime.count() >= activeTime::getInstance()->getActiveTime())   //超时置为无效
        {
            isValid = false;
        }
        else
        {
            //modify xqx 2020-6-5 17:55:19 增加数据判断（只判是否相等，数值量及字符串判断）

            if(Json::Value() == value)    //为空时代表不需要判断
            {
                //nothong to do
            }
            else
            {
                isValid = false;

                if(value.isInt() && mCurValue.isInt())
                {
                    if(value.asInt() == mCurValue.asInt())
                    {
                        isValid = true;
                    }
                }
                else if(value.isDouble() && mCurValue.isDouble())
                {
                    isValid = qFuzzyCompare(value.asDouble(), mCurValue.asDouble());
                }
                else if(value.isString() && mCurValue.isString())
                {
                    if(value.asString() == mCurValue.asString())
                    {
                        isValid = true;
                    }
                }
            }

            //end
        }
        return isValid;
    }

    void setValue(Json::Value value)
    {
        isValid = true;
        mCurValue = value;
    }

private:
    std::chrono::system_clock::time_point startTime;
    //int mActiveTime = 10000000;  ///参数有效时间(ms)
    bool isValid;           ///是否有效
    Json::Value mCurValue;       ///< 当前值
};


class virtualParams
{
public:
    static virtualParams *getInstance()
    {
        if(mInstance == nullptr)
        {
            mInMutex.lock();
            if(mInstance == nullptr)
            {
                mInstance = new virtualParams();
            }
            mInMutex.unlock();
        }

        return mInstance;
    }
private:
    using keyType = mapKey;
    using valueType = mapValue;

    using hashKeyType = QPair<QString, QString>;
    using hashValueType = mapValue*;
private:
    virtualParams();
public:
    void setValue(const keyType &key, Json::Value = Json::nullValue);
    bool isMeet(const keyType &key, Json::Value = Json::nullValue);
private:
    QHash<hashKeyType, hashValueType> mHashParamsManager;
#if USE_QT_LOCK
    QReadWriteLock mDataMutex;
#else
    std::mutex mDataMutex;
#endif
private:
    static virtualParams *mInstance;
    static std::mutex mInMutex;
public:
    static QMutex mParamsUpDateMutex;           ///< 参数更新锁
    static QWaitCondition mParamsUpdateCondition;   ///< 参数更新条件
};

#endif // VIRTUALPARAMS_H
