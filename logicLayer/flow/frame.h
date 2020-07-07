#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <tuple>
#include <memory>

#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../src/PfCommon/TinyXml/tinyxml.h"
#include "../src/PfCommon/jsoncpp/json.h"

//#define DEBUG_FRAME 0

class value;
class head;

#define FRAME_BE    "BE"
#define FRAME_FE    "FE"
#define FRAME_93    "密码集控装置协议"
#define FRAME_C3    "C3协议"
#define FRAME_MIDDLE    "中间件"
#define FRAME_XK        "信控协议"
#define FRAME_DM        "定位瞄准协议"
#define FRAME_CZXK      "车长显控通讯协议"
#define FRAME_M1553B    "M1553B协议"
#define FRAME_JG        "机柜协议"

/** 自定义消息 **/
#define XK_CUSTOM_MSG   "60000"
#define UI_CUSTOM_MSG   "80000001"
//#define DM_CUSTOM_MSG   "61000"


class frame
{
public:
    enum {
        Cmd,
        Param
    };

    /** 消息类型 **/
    enum msgType{
        GeneralMsg, ///< 通用消息
        CustomMsg   ///< 自定义消息
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
    msgType getMsgType(){return mCurMsgType;}
    /**
     * @brief getCustomMsg  获取自定义消息
     * @param table
     * @param table
     */
    void getCustomMsg(std::string &table, std::string &coding);
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> getCurFrameObj();
    void init(TiXmlElement *);
    void getFrameMsg(std::vector<unsigned char> &msg, bool &isAck, int resendCnt = 0);

    void getResendMsg(std::vector<unsigned char> &msg);

    void setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter);
    void setDevUuid(const std::string &uuid){mCurUuid = uuid;}
    Json::Value getRunItems();

    int getFrameType(){return mCurParamType;}

    bool is1553B();

    std::string get1553BModel();
    std::string get1553BBus();
    bool getBcModelInfo(int &rtAddr, int &saAddr);
    bool getRtModelInfo(int &sRt, int &sSa, int &rRt, int &rSa, int &size);
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
    Json::Value fillInfoOne(const unsigned short &table, const unsigned short &coding, const Json::Value &data, const int &dataType, const bool &isOver, const unsigned short &num);

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
    Json::Value fillInfoThree(int table, int coding, const Json::Value &data, std::string dataType, bool isOver, int devNum, int modelNum);

    Json::Value fillMiddleInfoWord0(int coding, int num, int dataType, int dataLen, Json::Value data, bool isOver, std::string reserve);
    Json::Value fillMiddleInfoWord1(int coding, int num, std::string reserve);
    Json::Value fillMiddleInfoWord2(int coding, int dNum, int devNum, int modelNum, int dataType, int dataLen, Json::Value data, bool isOver, std::string reserve);

    void setCustomMsgFromTable(std::string table);
private:
    using paramsType = std::tuple<std::string, std::string, std::shared_ptr<value>>;
    std::vector<paramsType> mParamsVec;
    std::shared_ptr<head> mHeadObj;
    int mCurParamType;
    std::map<std::string, std::shared_ptr<Pf::PfIcdWorkBench::frameObj>> mIcdFrameObj;
    std::string mCurFrameType;
    msgType mCurMsgType;
    std::string mCurUuid;
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

class head1553B : public head
{
public:
    void init(TiXmlElement *) override;
    Json::Value serialize() override;
    std::string frameType() override {return FRAME_M1553B;}
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

class headJg : public head
{
public:
    void init(TiXmlElement *) override;
    Json::Value serialize() override;
    std::string frameType() override {return FRAME_JG;}
private:
    Json::Value mJsonV;
};

class headCzxk : public head
{
public:
    void init(TiXmlElement *) override;
    Json::Value serialize() override;
    std::string frameType() override {return FRAME_CZXK;}
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
