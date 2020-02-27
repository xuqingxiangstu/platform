#ifndef KEYPARAMSHOWSTEP_H
#define KEYPARAMSHOWSTEP_H

#include "keyParamShowStep_global.h"

#include "../runObj/runobj.h"

class KEYPARAMSHOWSTEPSHARED_EXPORT keyParamShowStep : public runObj
{
public:
    keyParamShowStep();
    ~keyParamShowStep();
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
    KEYPARAMSHOWSTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // KEYPARAMSHOWSTEP_H
    
    