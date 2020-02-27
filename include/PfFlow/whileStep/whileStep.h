#ifndef WHILESTEP_H
#define WHILESTEP_H

#include "whileStep_global.h"

#include "../runObj/runobj.h"

#include "../condition/condition.h"

#include <memory>


class WHILESTEPSHARED_EXPORT whileStep : public runObj
{
public:
    whileStep();
    ~whileStep();
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
    WHILESTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // WHILESTEP_H
    
    
