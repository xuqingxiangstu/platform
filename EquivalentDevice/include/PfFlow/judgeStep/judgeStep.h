#ifndef JUDGESTEP_H
#define JUDGESTEP_H
/**
 * @file judgeStep.h
 * @brief The judgeStep class
 * 判断流程步
 * @author fjt
 * @date 2020-02-21
 * @version V1.0
 */
#include "judgeStep_global.h"

#include "../runObj/runobj.h"

/** 版本号 **/
#define VERSION "1.0.0"


class JUDGESTEPSHARED_EXPORT judgeStep : public runObj
{
public:
    judgeStep();
    ~judgeStep();
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
    /**
     * @brief getVarV 获取引用的值
     * @param use 引用的字符串
     * @return
     */
    std::string getVarV(std::string use);
private:
    runObj *mRunObj;    ///< 运行类
    bool mResult; ///< 测试结果
    Pf::PfAdapter::PfAdapterManager *mAdapter; ///< 适配器管理
    std::string strOutText; ///< 输出信息
    std::string mstrexp; ///计算表达式
    std::string mstrCalcResult;///表达式的计算结果
    std::string mstrVContent;///变量v的内容
    std::string mstrVValue; //变量V的值
};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    JUDGESTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // JUDGESTEP_H
    
    
