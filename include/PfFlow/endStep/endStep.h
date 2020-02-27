#ifndef ENDSTEP_H
#define ENDSTEP_H

#include "endStep_global.h"

#include "../runObj/runobj.h"

class ENDSTEPSHARED_EXPORT endStep : public runObj
{
public:
    endStep();
    ~endStep();
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
};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    ENDSTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // ENDSTEP_H
    
    