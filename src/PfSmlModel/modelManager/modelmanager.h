#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "modelmanager_global.h"

/**
 * @file pfadapter.h
 * @brief 仿真模型抽象类
 * 通过该类可实现随机数、曲线、温度变化仿真数据接口抽象
 * @author jj
 * @date 2020-01-03
 * @copyright (C)2020 BJTU
 */

#include "modelmanager_global.h"
#include "../algorithm/Algorithm.h"

#include <unordered_map>
#include <memory>

#include <sstream>

/**
 * @brief The ModelManager class
 * 随机数、曲线、温度变化仿真数据抽象管理类，由唯一设备ID索引
 * @note xml文件配置如下
 * @code
 *  <?xml version="1.0"?>
 *    <!-- 根据各仿真模型配置进行填充，具体配置详见各适配器init注解-->
 *    <simulation devId="1" channel="1" model="waveformAlgorithm">
 *    </simulation>
 * @endcode
 */
class MODELMANAGERSHARED_EXPORT ModelManager
{
public:
    ModelManager();
    ~ModelManager();
public:
    /**
     * @brief init  初始化
     * @param xmlPath 文件路径
     * @exception catch(runtime_error)
     */
    void init(const std::string &xmlPath);

    /**
     * @brief getAlgorithm    获取仿真模型
     * @param[in] id 设备ID
     * @param[out] algorithm  仿真模型
     * @return 是否成功
     * - true：成功
     * - false：失败
     */
    bool getAlgorithm(const std::string &id, Algorithm **algorithm);

    /**
     * @brief setAlgorithm    设置仿真模型
     * @param[in] id 设备ID
     * @param[in] algorithm 句柄
     */
    void setAlgorithm(const std::string &id, Algorithm *algorithm);
private:
    std::unordered_map<std::string, std::shared_ptr<Algorithm>> mManagement; ///<  仿真模型管理

};

#endif // MODELMANAGER_H
