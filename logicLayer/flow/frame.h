#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <tuple>
#include <memory>

#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../src/PfCommon/TinyXml/tinyxml.h"
#include "../src/PfCommon/jsoncpp/json.h"

#define DEBUG_FRAME 0

class value;
class head;

#define FRAME_BE    "BE"
#define FRAME_FE    "FE"
#define FRAME_93    "93"
#define FRAME_MIDDLE    "中间件"

class frame
{
public:
    enum {
        Cmd,
        Param
    };
private:
    enum{
        InfoWord_One = 0,
        InfoWord_Two,
        InfoWord_Three
    };

    enum{
        Cmd_Table_Index,
        Cmd_Coding_Index
    };

    enum{
        Param_Table_Index,
        Param_Coding_Index,
        Param_Value_Index
    };
public:
    frame();
public:
    void init(TiXmlElement *);
    void getFrameMsg(std::vector<unsigned char> &msg, bool &isAck, int resendCnt = 0);

    void getResendMsg(std::vector<unsigned char> &msg);

    void setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter);

    Json::Value getRunItems();

    int getFrameType(){return mCurParamType;}
private:
    /**
     * @brief fill    数据域填充
     * @param frameType     帧类型(93/FE/BE)
     * @param infoWordType  信息字类型（BE帧时有效 1/2/3）
     * @return 数据域Json
     */
    Json::Value fill(const std::string &frameType, const std::string &infoWordType);

    Json::Value fill93();
    Json::Value fillFe();
    Json::Value fillBe(const std::string &infoWordType);
    Json::Value fillMiddle(const std::string &infoWordType);
    Json::Value fillRegion();

    /**
     * @brief fillInfoOne   填充信息字1
     * @param table     表号
     * @param coding    编码
     * @param v         值
     * @param isOver    是否超差
     * - true：超差
     * - false：不超差
     * @param num   弹编号/2号车编号
     * @return
     */
    Json::Value fillInfoOne(const unsigned short &table, const unsigned short &coding, const double &v, const bool &isOver, const unsigned short &num);

    /**
     * @brief fillInfoTwo   填充信息字2
     * @param table     表号
     * @param coding    编码
     * @return
     */
    Json::Value fillInfoTwo(const unsigned short &table, const unsigned short &coding);

    /**
     * @brief fillInfoThree 填充信息字3
     * @param table     表号
     * @param coding    编码
     * @param v         值
     * @param dataType  数据类型
     * - 1：数值
     * - 2：状态
     * - 3：字符串
     * @param isOver    是否超差
     * - true：超差
     * - false：不超差
     * @param devNum    设备序号
     * @param modelNum  模块序号
     * @return JSON
     */
    Json::Value fillInfoThree(int table, int coding, std::string v, std::string dataType, bool isOver, int devNum, int modelNum);

    Json::Value fillMiddleInfoWord0(int coding, int num, int dataType, int dataLen, Json::Value data, bool isOver, std::string reserve);
    Json::Value fillMiddleInfoWord1(int coding, int num, std::string reserve);
    Json::Value fillMiddleInfoWord2(int coding, int dNum, int devNum, int modelNum, int dataType, int dataLen, Json::Value data, bool isOver, std::string reserve);
private:
    using paramsType = std::tuple<std::string, std::string, std::shared_ptr<value>>;
    std::vector<paramsType> mParamsVec;
    std::shared_ptr<head> mHeadObj;
    int mCurParamType;
    std::map<std::string, std::shared_ptr<Pf::PfIcdWorkBench::frameObj>> mIcdFrameObj;
    std::string mCurFrameType;

};

class Algorithm;

class value
{
public:
    using valueType = std::string;
    value();
    value(valueType v):mInitValue(v){}
public:
    void init(TiXmlElement *);
    std::string getValue();
private:
    valueType mInitValue;
    std::shared_ptr<Algorithm> mChangeModel;
};

class head
{
public:
    virtual void init(TiXmlElement *){}
    virtual Json::Value serialize(){return nullptr;}
    virtual std::string frameType(){return "";}
};

class headBe : public head
{
public:
    void init(TiXmlElement *) override;
    Json::Value serialize() override;
    std::string frameType() override {return FRAME_BE;}
private:
    Json::Value mJsonV;
};

class headFe : public head
{
public:
    void init(TiXmlElement *) override;
    Json::Value serialize() override;
    std::string frameType() override {return FRAME_FE;}
private:
    Json::Value mJsonV;
};

class head93 : public head
{
public:
    void init(TiXmlElement *) override;
    Json::Value serialize() override;
    std::string frameType() override {return FRAME_93;}
private:
    Json::Value mJsonV;
};

class headMiddle : public head
{
public:
    void init(TiXmlElement *) override;
    Json::Value serialize() override;
    std::string frameType() override {return FRAME_MIDDLE;}
private:
    Json::Value mJsonV;
};

#endif // FRAME_H
