#ifndef WAVEFORMMODEL_H
#define WAVEFORMMODEL_H

#include "waveformModel_global.h"

#include "../algorithm/Algorithm.h"

#include <memory>

/** 版本号 **/
#define VERSION "1.0.0"

/**
 * @brief The waveformModel class
 * 曲线仿真模型
 * @note xml配置如下
 * @code
 *  <!--
 *      曲线数仿真模型配置
 *      min：最小值
 *      max：最大值
 *  -->
 *  <dev id="send" model="randaModel">
 *  	<!-- 设备名称，Ni-Max中查看 -->
 *  	<rname>send</rname>
 *  	<!--振幅-->
 *      <a>4</a>
 *      <!-- 频率 -->
 *      <w>9</w>
 *      <!-- 平移量 -->
 *      <f>9</f>
 *      <!-- 初相 -->
 *      <y>9</y>
 *  </dev>
 * @endcode
*/
class WAVEFORMMODELSHARED_EXPORT waveformModel : public Algorithm
{
public:
  waveformModel();
  ~waveformModel();
public:
  void init(const TiXmlElement *ele) override;
  void init(std::initializer_list<double> params) override;
  int setAttribute(const std::string &attr, const void *value) override;
  std::string version() override {return VERSION;}
  std::string getClassName() override;  
  valueType value();
private:
  void calculate();
  double a;   ///< 振幅
  double w;   ///< 频率
  double f;   ///< 平移量
  double y;   ///< 初相
  double mXValue;
  valueType mCurValue;   ///< 当前数值
  std::mutex mMutex;   ///< 消息锁
};

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          WAVEFORMMODELSHARED_EXPORT Algorithm *LoadClass();
      }

#endif // WAVEFORMMODEL_H
    
    
