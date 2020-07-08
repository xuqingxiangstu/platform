#ifndef S93BUSINESS_H
#define S93BUSINESS_H


/**
  *93帧业务相关
  */

#include "../business.h"

class s93Business : public business
{
public:
    s93Business();
public:
    void deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &msg) override;
    void setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort) override;
    void setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj) override;
    void setRecordUuid(const std::string &uuid) override {mRecordUuid = uuid;}
    void setDevUuid(const std::string &uuid) override {mDevUuid = uuid;}
    std::shared_ptr<business> clone() override;
private:
    void toUi(const std::string &msg, bool state = true);

    std::string getVirtualCoding(unsigned char table);
private:
    Pf::PfAdapter::Adapter *mBusObj;
    Pf::PfAdapter::Adapter *mUiBus;
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    std::string mDstIp;
    std::string mRecordUuid;
    std::string mDevUuid;
    int mDstPort;
    const std::string mViratulTableNum = "80000002";///< 虚拟表号（与数据库一致）
};

#endif // S93BUSINESS_H
