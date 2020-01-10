#ifndef PRINTFSTEP_H
#define PRINTFSTEP_H

#include "printfStep_global.h"

#include "../runObj/runobj.h"

#define BUF_SIZE 1024

class PRINTFSTEPSHARED_EXPORT printfStep : public runObj
{
public:
    printfStep();
    ~printfStep();
public:
    void init(const TiXmlElement *ele) override;
    std::string getClassName() override;
    bool exec() override;
    std::string getVarValue(std::string) override;
    bool getResult() override;
    std::string outText() override;
    void setAutoRunObj(runObj *obj) override;
    void setAdapter(Pf::PfAdapter::PfAdapterManager *adapter) override;
private:
    void toUi(std::string msg, std::string status);
    std::string getVarV(std::string use);
private:
    runObj *mRunObj;    ///< 运行类
    bool mResult; ///< 测试结果
    Pf::PfAdapter::PfAdapterManager *mAdapter; ///< 适配器管理
    std::string strOutText; ///< 输出信息
private:
    std::string mFormat;
    std::vector<std::string> strArgs;
    std::string msgType;

    char strTmpBuf[BUF_SIZE];
};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    PRINTFSTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // PRINTFSTEP_H
    
    
