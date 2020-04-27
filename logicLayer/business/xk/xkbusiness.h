#ifndef XKBUSINESS_H
#define XKBUSINESS_H

#include "../business.h"

/**
 * @brief The xkBusiness class  信控业务相关
 */

#define ZY_STAT 0x91
#define ZY_STOP 0x9E

#define ZY_DOWN_FILE_TYPE       0xF1    ///< 下发文件
#define ZY_FILE_INFO_TYPE       0xF2    ///< 文件信息
#define ZY_REQUEST_DATA_TYPE    0xF5    ///< 文件数据请求
#define ZY_TRANS_OVER_TYPE      0xF6    ///< 文件传输结束
#define ZY_DATA_TYPE            0xF7    ///< 文件数据帧


class xkBusiness : public business
{
public:
    xkBusiness();
public:
    void deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &msg) override;
    void setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort) override;
    void setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj) override;
    void setRecordUuid(const std::string &uuid) override {mRecordUuid = uuid;}
    std::shared_ptr<business> clone() override;
private:
    void zy(const Json::Value &result);
    void fileInfo(const Json::Value &result);
    void requestData(const Json::Value &result);
    void fileData(const Json::Value &result);
    void testFlow(const Json::Value &result);
    void toUi(const std::string &msg, bool state);
public:
    static std::string getFlowName(int type);
private:
    Pf::PfAdapter::Adapter *mBusObj;
    Pf::PfAdapter::Adapter *mUiBus;
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    std::string mDstIp;
    std::string mRecordUuid;
    int mDstPort;   
};

#endif // ZYBUSINESS_H
