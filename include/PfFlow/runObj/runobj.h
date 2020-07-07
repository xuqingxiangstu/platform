#ifndef RUNOBJ_H
#define RUNOBJ_H

/**
 * @file runobj.h
 * @brief 流程部基类
 * @author xqx
 * @date 2020-01-09
 * @version V1.0
 */

#include "../../PfCommon/TinyXml/tinyxml.h"
#include "../../PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../../PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"

#include <string>

#define ProcedureNode   "TestProcedure"
#define DescribeNode    "Describe"
#define TestCaseNode    "TestCase"
#define TestStepNode    "TestStep"
#define StepTypeAttr    "StepType"
#define NodeEnableAttr  "NodeEnable"
#define KeyAttr         "Index"

#define VAR_NAME(var) (#var)

#define DEBUG_OBJ 0

/**
 * @brief The runObj class
 * 流程部运行基类
 */
class runObj
{    
public:
    runObj(){}
    virtual ~runObj(){}
public:
    /**
     * @brief init 初始化流程部
     * @param[in] ele xml节点
     */
    virtual void init(const TiXmlElement *ele){}

    /**
     * @brief getClassName 获取类名称
     * @return 类名
     */
    virtual std::string getClassName(){ return ""; }

    /**
     * @brief exec 流程部执行体
     * @return 是否成功
     * - true：成功
     * - false：失败
     */
    virtual bool exec() {return false;}

    /**
     * @brief getVarValue 获取变量值
     * @return 值
     */
    virtual std::string getVarValue(std::string){return "";}

    /**
     * @brief getObj    获取指定流程部
     * @param proceIndex
     * @param testCaseIndex
     * @param stepIndex
     * @return 流程部
     */
    virtual runObj *getObj(std::string proceIndex, std::string testCaseIndex, std::string stepIndex){return nullptr;}

    /**
     * @brief getResult 获取运行结果
     * @return 执行是否成功
     * - true：成功
     * - false：失败
     */
    virtual bool getResult() { return false; }

    /**
     * @brief outText 流程部输出信息
     * @note：流程部执行exe之后待输出的信息
     * @return 信息
     */
    virtual std::string outText() { return ""; }

    /**
     * @brief setAutoRunObj 设置运行句柄
     * @param[in] obj 句柄
     */
    virtual void setAutoRunObj(runObj *obj){}

    /**
     * @brief setAdapter 设置适配器句柄
     * @param[in] adapter 适配器
     */
    virtual void setAdapter(Pf::PfAdapter::PfAdapterManager *adapter){}

    /**
     * @brief setIcdframeadapter 设置ICD帧管理（解析仿真）句柄
     * @param icdAdapter ICD帧管理
     */
    virtual void setIcdframeadapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter){}

    /**
     * @brief version 版本号
     * @return 版本号
     */
    virtual std::string version(){return "";}
};

/** 类导出函数指针 **/
typedef runObj *(*LOAD_STEP_LIB)();

#endif // RUNOBJ_H
