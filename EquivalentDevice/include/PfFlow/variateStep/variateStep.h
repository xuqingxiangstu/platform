#ifndef VARIATESTEP_H
#define VARIATESTEP_H
/**
 * @file variateStep.h
 * @author fjt
 * @version 0.1
 * @date 2020-02-12
 * @copyright Copyright (c) 2020
 * @todo 弄清楚类型的解析如何使用，暂没使用该参数
 */
#include "variateStep_global.h"

#include "../runObj/runobj.h"
/** 版本号 **/
#define VERSION "1.0.0"
/**
 * @brief The variateStep class
 * 变量流程步，该变量支持引用，或配置的时候赋值
 */

class VARIATESTEPSHARED_EXPORT variateStep : public runObj
{
public:
    variateStep();
    ~variateStep();
public:
    /**
     * @brief init 初始化
     * @param ele 该流程的xml描述文件
     */
    void init(const TiXmlElement *ele) override;
    /**
     * @brief getClassName 获取类的名字
     * @return 本类的名字
     */
    std::string getClassName() override;
    /**
     * @brief exec 该流程步的执行函数
     * @return true表示成功，false表示失败
     */
    bool exec() override;
    /**
     * @brief getVarValue 获得变量的值
     * @return 返回变量值的字符串
     */
    std::string getVarValue(std::string) override;
    /**
     * @brief getResult获取执行结果
     * @return true表示成功，false表示失败
     */
    bool getResult() override;
    /**
     * @brief outText 输出信息函数
     * @return 输出的内容
     */
    std::string outText() override;
    /**
     * @brief setAutoRunObj 设置自动运行实例
     * @param obj 执行对象
     */
    void setAutoRunObj(runObj *obj) override;
    /**
     * @brief setAdapter 设置适配器
     * @param adapter 适配对象
     */
    void setAdapter(Pf::PfAdapter::PfAdapterManager *adapter) override;
    /**
     * @brief version 获取该流程步的版本
     * @return 返回版本号
     */
    std::string version() override {return VERSION;}
private:
    /**
     * @brief getVarV 获取变量的值
     * @param use 变量的名字
     * @return
     */
    std::string getVarV(std::string use);
private:
    runObj *mRunObj;    ///< 运行类
    bool mResult; ///< 测试结果
    Pf::PfAdapter::PfAdapterManager *mAdapter; ///< 适配器管理
    std::string strOutText; ///< 输出信息
    std::string strValue; ///< 变量值的内容
    std::string mstrValueType;///< 变量值的类型
    std::string mvarValue; ///< 变量的值
};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    VARIATESTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // VARIATESTEP_H
    
    
