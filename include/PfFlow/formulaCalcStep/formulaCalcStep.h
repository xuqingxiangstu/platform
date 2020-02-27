#ifndef FORMULACALCSTEP_H
#define FORMULACALCSTEP_H

#include "formulaCalcStep_global.h"

#include "../runObj/runobj.h"
/** 版本号 **/
#define VERSION "1.0.0"


class FORMULACALCSTEPSHARED_EXPORT formulaCalcStep : public runObj
{
public:
    formulaCalcStep();
    ~formulaCalcStep();
public:
    void init(const TiXmlElement *ele) override;
    std::string getClassName() override;
    bool exec() override;
    std::string getVarValue(std::string) override;
    bool getResult() override;
    std::string outText() override;
    void setAutoRunObj(runObj *obj) override;
    void setAdapter(Pf::PfAdapter::PfAdapterManager *adapter) override;
    std::string version() override {return VERSION;}
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
    FORMULACALCSTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // FORMULACALCSTEP_H
    
    