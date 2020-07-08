#ifndef CZXKBUSINESS_H
#define CZXKBUSINESS_H

/**
  *车长显控业务相关
  */

#include <QStringList>

#include "../business.h"

class czxkBusiness : public business
{
    QStringList testName = {
        "",
        "发射阵地电源控制",
        "技术阵地电源控制",
        "",
        "弹上配电控制",
        "等效器检查",
        "发射车控制",
        "",
        "",
        "",
        "",
        "天文时钟设置",
        "",
        "",
        "发射阵地综合检查",
        "发射阵地模飞",
        "IMU-WYZ检查",
        "三自惯组自标定",
        "收星检查",
        "",
        "导引头测试",        
        "伺服系统测试",
        "",
        "",
        "",
        "",
        "",
        "技术阵地模飞",        
        "正俯仰通道安全自毁测试",
        "负俯仰通道安全自毁测试",
        "正偏航通道安全自毁测试",
        "负偏航通道安全自毁测试",
        "极性检查",
        "遥测弹发射流程",
        "诸元数据查看",
        "历史测试数据管理",
        "查询硬盘剩余空间",
        "显控软件信息查询",
        "诸元数据输入",
        "弹上软件更新"
    };
public:
    czxkBusiness();
public:
    void deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &msg) override;
    void setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort) override;
    void setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj) override;
    void setRecordUuid(const std::string &uuid) override {mRecordUuid = uuid;}
    void setDevUuid(const std::string &uuid) override {mDevUuid = uuid;}
    std::shared_ptr<business> clone() override;
private:
    void respondCmd(int frameType, int code, Json::Value array, unsigned char srcNode, unsigned char dstNode);
    void toUi(const std::string &msg, bool state = true);
    std::string getCmdName(int type);
    bool getTableAndCoding(int type, std::string &table, std::string &coding);
private:
    void exeCmd(Json::Value regionJs, unsigned char srcNode, unsigned char dstNode);

    void testInfo(Json::Value regionJs, unsigned char srcNode, unsigned char dstNode);

    void queryVersion(Json::Value regionJs, unsigned char srcNode, unsigned char dstNode);

    void checkCmd(Json::Value regionJs, unsigned char srcNode, unsigned char dstNode);

    void sendFsCmd(Json::Value regionJs, unsigned char srcNode, unsigned char dstNode);
private:
    Pf::PfAdapter::Adapter *mBusObj;
    Pf::PfAdapter::Adapter *mUiBus;
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    std::string mDstIp;
    std::string mRecordUuid;
    std::string mDevUuid;
    int mDstPort;
    const std::string mViratulTableNum = "80000000";///< 虚拟表号（与数据库一致）
};


#endif // CZXKBUSINESS_H
