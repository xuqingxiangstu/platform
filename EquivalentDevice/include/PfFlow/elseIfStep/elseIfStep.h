#ifndef ELSEIFSTEP_H
#define ELSEIFSTEP_H

#include "elseIfStep_global.h"

#include "../runObj/runobj.h"

#include "../condition/condition.h"

#include <memory>

class ELSEIFSTEPSHARED_EXPORT elseIfStep : public runObj
{
public:
    elseIfStep();
    ~elseIfStep();
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
    std::shared_ptr<Condition> mCondition;
};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    ELSEIFSTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // ELSEIFSTEP_H
    
    
