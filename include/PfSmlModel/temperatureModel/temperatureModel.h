#ifndef TEMPERATUREMODEL_H
#define TEMPERATUREMODEL_H

#include "temperatureModel_global.h"

#include "../algorithm/Algorithm.h"

#include <memory>

/** 版本号 **/
#define VERSION "1.0.0"
/** 温度变化类型(上升) **/
#define RISING  "Rising"
/** 温度变化类型(下降) **/
#define FALLING "Falling"
/** 温度变化类型(保持不变) **/
#define KEEP    "Keep"

/**
 * @brief The temperatureModel class
 * 温度变化仿真模型
 * @note xml配置如下
 * @code
 *  <!--
 *      温度变化仿真模型配置
 *      min：最小值
 *      max：最大值
 *      initValue：初始值
 *      changeStep：步长
 *      changeType：变化类型(Rising-Falling-Keep)
 *  -->
 *  <dev id="send" model="temperatureModel">
 *      <!-- 设备名称，Ni-Max中查看 -->
 *      <rname>send</rname>
 *      <!--最小值-->
 *      <min>4</min>
 *      <!-- 最大值 -->
 *      <max>9</max>
 *      <!-- 初始值 -->
 *      <initValue>9</initValue>
 *      <!-- 步长 -->
 *      <changeStep>9</changeStep>
 *      <!-- 变化类型(Rising-Falling-Keep) -->
 *      <changeType>9</changeType>
 *      </dev>
 * @endcode
*/
class TEMPERATUREMODELSHARED_EXPORT temperatureModel : public Algorithm
{
public:
    ///温度变化类型
    enum tpType
    {
        Rising, ///< 上升
        Falling, ///< 下降
        Keep      ///<保持不变
    };
  temperatureModel();
  ~temperatureModel();
public:
  void init(const TiXmlElement *ele) override;
  void init(std::initializer_list<double> params) override;
  int setAttribute(const std::string &attr, const void *value) override;
  std::string version() override {return VERSION;}
  std::string getClassName() override;
  valueType value() override;
private:
  /**
   * @brief setInitValue   设置当前值
   * @param v              值
   */
  void setInitValue(valueType v);
  /**
   * @brief setChangeType   设置变化类型
   * @param type            类型
   */
  void setChangeType(tpType type);
  void calculate();
  valueType mCurValue;   ///< 当前数值
  tpType mTpType;   ///< 变化类型
  valueType mChangeStep;  ///< 变化步长
  valueType mMaxValue;    ///< 最大值
  valueType mMinValue;    ///< 最小值
  std::mutex mMutex;   ///< 消息锁
};

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          TEMPERATUREMODELSHARED_EXPORT Algorithm *LoadClass();
      }

#endif // TEMPERATUREMODEL_H
    
    
