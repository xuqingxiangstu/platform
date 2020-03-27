#ifndef CONDITION_H
#define CONDITION_H


#include "../src/PfCommon/TinyXml/tinyxml.h"
#include "../src/PfCommon/jsoncpp/json.h"

#include <vector>
#include <tuple>


///启动条件
class startCondition
{
public:
    void init(TiXmlElement *);
    bool isConform();
    static std::string className(){return "startCondition";}
    Json::Value getRunItems();
private:
    //std::vector<std::tuple<inValueType, outValueType>> mParams;
    std::string mId;
    std::string mTable;
    std::string mCoding;
};

///停止条件
class stopCondition
{
public:
    void init(TiXmlElement *);
    bool isConform();
    static std::string className(){return "stopCondition";}
private:
    //std::vector<std::tuple<inValueType, outValueType>> mParams;
    std::string mId;
    std::string mTable;
    std::string mCoding;
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
