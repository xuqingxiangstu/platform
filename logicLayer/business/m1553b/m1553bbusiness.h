#ifndef M1553BBUSINESS_H
#define M1553BBUSINESS_H


/**
  *1553帧业务相关
  */

#include "../business.h"

class m1553bBusiness : public business
{
public:
    m1553bBusiness();
public:
    void deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &msg) override;
    void setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort) override;
    void setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj) override;
    void setRecordUuid(const std::string &uuid) override {mRecordUuid = uuid;}
    void setDevUuid(const std::string &uuid) override {mDevUuid = uuid;}
    void setParam(const Json::Value &param) override;
private:
    void toUi(const std::string &msg, bool state = true);

    void exeCmd(const Pf::PfIcdWorkBench::byteArray &inData);
private:
    Pf::PfAdapter::Adapter *mBusObj;
    Pf::PfAdapter::Adapter *mUiBus;
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    std::string mDstIp;
    std::string mRecordUuid;
    std::string mDevUuid;
    int mDstPort;
    unsigned short mRtAddr;
    unsigned short mSaAddr;
    unsigned short mCmdCnt;///< 命令计数
};

#endif // M1553BBUSINESS_H
