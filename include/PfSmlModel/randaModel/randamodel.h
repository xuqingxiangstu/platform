#ifndef RANDAMODEL_H
#define RANDAMODEL_H

#include "randamodel_global.h"
#include "../algorithm/Algorithm.h"
#include <memory>

/**
 * @brief The RandaModel class
 * 随机数仿真模型
 * @note xml配置如下
 * @code
 *  <!--
 *      随机数仿真模型配置
 *      min：最小值
 *      max：最大值
 *  -->
 *  <dev id="send" model="randaModel">
 *      <!--最小值-->
 *      <min>4</min>
 *      <!-- 最大值 -->
 *      <max>9</max>
 *  </dev>
 * @endcode
*/
class RANDAMODELSHARED_EXPORT RandaModel : public Algorithm
{
public:
    RandaModel();
    ~RandaModel();
public:
    void init(const TiXmlElement *ele) override;
    void init(std::initializer_list<double> params) override;
    int setAttribute(const std::string &attr, const void *value) override;
    std::string getClassName() override;
    std::string version() override { return ""; }
    valueType value();
private:
    void calculate();
    std::shared_ptr<RandaModel> randm; ///< 随机数模型实例
    std::mutex mMutex;   ///< 消息锁
private:
    valueType mCurValue;   ///< 当前数值

    valueType mMinValue;   ///< 最小值
    valueType mMaxValue;   ///< 最大值

    const int mDecimal = 100000;
};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    RANDAMODELSHARED_EXPORT Algorithm *LoadClass();
}

#endif // RANDAMODEL_H
