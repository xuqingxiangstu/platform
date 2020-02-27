#ifndef JUDGEMULTISTEP_H
#define JUDGEMULTISTEP_H
/**
 * @file judgeMultiStep.h
 * @brief The judgeMultiStep class
 * 多值判断流程步
 * @author fjt
 * @date 2020-02-21
 * @version V1.0
 */
#include "judgeMultiStep_global.h"

#include "../runObj/runobj.h"
/** 版本号 **/
#define VERSION "1.0.0"
#include <tuple>
#include <vector>

class JUDGEMULTISTEPSHARED_EXPORT judgeMultiStep : public runObj
{
public:
    judgeMultiStep();
    ~judgeMultiStep();
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
    /** 存储变量信息的向量，每个变量分别包括名字、引用内容/内容、获取引用后的内容*/
    std::vector<std::tuple<std::string,std::string,std::string>> mvecVariate;
    std::string mstrexp;///< 表达式
    std::string mstrCalcResult;///< 计算接收
};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    JUDGEMULTISTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // JUDGEMULTISTEP_H
    
    
