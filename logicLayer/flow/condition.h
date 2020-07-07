#ifndef CONDITION_H
#define CONDITION_H


#include "../src/PfCommon/TinyXml/tinyxml.h"
#include "../src/PfCommon/jsoncpp/json.h"
#include "../virtualParams/virtualparams.h"
#include <vector>
#include <mutex>
#include <tuple>
#include <QDebug>

///启动条件
class startCondition
{
public:
    startCondition(){}
    ~startCondition(){
        qDebug() << "~startCondition";
    }
public:    
    void init(TiXmlElement *);
    bool isConform();
    static std::string className(){return "startCondition";}
    Json::Value getRunItems();
    QString getId(){return mId;}
    QString getTable(){return mTable;}
    QString getCoding(){return mCoding;}
    Json::Value getValue(){return mValue;}
private:   
    QString mId;
    QString mTable;
    QString mCoding;
    Json::Value mValue;
    std::mutex mMutex;
    mapKey mConditionKey;

};

///停止条件
class stopCondition
{
public:
    void init(TiXmlElement *);
    bool isConform();
    static std::string className(){return "stopCondition";}
private:   
    QString mId;
    QString mTable;
    QString mCoding;
    Json::Value mValue;
    std::mutex mMutex;
    mapKey mConditionKey;
};

//时机
class timing
{
public:
    enum timingType
    {
        CYCLE = 0,  //循环发送
        NOW,    //立即执行
        MANUAL,  //手动触发
        TIMING_SIZE
    };
private:
    std::string timingStr[TIMING_SIZE] = {
        "周期",
        "立即",
        "手动"
    };
public:
    void init(TiXmlElement *);
    timingType getType(){return mTimingType;}
    std::string getTimingStr(){return timingStr[mTimingType];}
    static std::string className(){return "timing";}
private:
    timingType mTimingType;
};

#endif // CONDITION_H
