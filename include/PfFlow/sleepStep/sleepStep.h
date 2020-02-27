#ifndef SLEEPSTEP_H
#define SLEEPSTEP_H

#include "sleepStep_global.h"

#include "../runObj/runobj.h"

class SLEEPSTEPSHARED_EXPORT sleepStep : public runObj
{
public:
    sleepStep();
    ~sleepStep();
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
    runObj *mRunObj;    ///< 运行类
    bool mResult; ///< 测试结果
    Pf::PfAdapter::PfAdapterManager *mAdapter; ///< 适配器管理
    std::string strOutText; ///< 输出信息
private:
    int mTimeOut;
    std::string mUnit;
};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    SLEEPSTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // SLEEPSTEP_H
    
    
