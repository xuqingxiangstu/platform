#ifndef ALGORITHM_H
#define ALGORITHM_H


/**
 * @file algorithm.h
 * @brief 数据变化算法抽象基类
 * @author xqx
 * @date 2020-01-03
 * @copyright (C)2020 BJTU
 */

#include "../../PfCommon/TinyXml/tinyxml.h"
#include <string>
#include <mutex>
#include <initializer_list>

/** 获取变量名称 **/
#define VAR_NAME(var) (#var)

/** 数据处理类型 **/
using valueType = double;

/**
 * @brief The Algorithm class
 * 仿真模型基类接口
 */
class Algorithm
{
public:
    Algorithm(){}
    ~Algorithm(){}
public:
    /**
     * @brief init  初始化接口
     * @param[in] ele xml元素节点
     */
    virtual void init(const TiXmlElement *ele) {}
    /**
     * @brief setAttribute  设置属性值
     * @param[in] attr 属性
     * @param[in] value 值
     * @return 是否正确
     */
    virtual int setAttribute(const std::string &attr, const void *value){return 0;}
    /**
     * @brief getClassName  获取类名称
     * @return 类名称
     */
    virtual std::string getClassName(){return "";}

    /**
     * @brief init  根据输入参数初始化模型
     * @param[in] params 输入参数
     */
    virtual void init(std::initializer_list<double> params) {}
    /**
     * @brief init  获取当前数值
     * @return 当前数值
     */
    virtual valueType value(){ return 0; }


protected:
    /**
     * @brief version 版本号
     * @return 版本号
     */
    virtual std::string version(){return "";}

    const valueType unitConver = 1000.0;

};
/** 类导出函数指针 **/
typedef Algorithm *(*LOAD_STEP_LIB)();

#endif // ALGORITHM_H
