#ifndef SETSWITCHOUTSTEP_H
#define SETSWITCHOUTSTEP_H

#include "setSwitchOutStep_global.h"

#include "../runObj/runobj.h"

class SETSWITCHOUTSTEPSHARED_EXPORT setSwitchOutStep : public runObj
{
public:
    setSwitchOutStep();
    ~setSwitchOutStep();
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
    SETSWITCHOUTSTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // SETSWITCHOUTSTEP_H
    
    