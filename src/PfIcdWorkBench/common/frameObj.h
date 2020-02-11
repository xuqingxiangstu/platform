#ifndef FRAMEOBJ_H
#define FRAMEOBJ_H

/**
 * @file frameObj.h
 * @brief 帧仿真与解析基类
 * @author xqx
 * @date 2020-02-10
 * @version V1.0
 */

#include "../../PfCommon/TinyXml/tinyxml.h"

#include "./type.h"

#include <string>
#include <vector>
#include <memory>


/** 帧配置跟目录 **/
#define ROOT_PATH "./frameCfg/"

namespace Pf
{
    /**
     * @brief icd仿真解析
     */
    namespace PfIcdWorkBench
    {
        /**
         * @brief The frameObj class
         * 帧仿真与解析基类
         */
        class frameObj
        {
        public:
            frameObj(){}
            virtual ~frameObj(){}
        public:
            /**
             * @brief init 初始化流程部
             * @param[in] ele xml节点
             */
            virtual void init(const TiXmlElement *ele){}

            /**
             * @brief clone 类克隆（深拷贝）
             * @return 类句柄
             */
            virtual std::shared_ptr<frameObj> clone(){return nullptr;}

            /**
             * @brief getFrameName 获取帧名称
             * @return 帧名
             */
            virtual std::string getFrameName(){ return ""; }

            /**
             * @brief version 版本号
             * @return 版本号
             */
            virtual std::string version(){return "";}

            /**
             * @brief simulation    协议仿真，根据配置协议进行参数仿真，组成一帧数据
             * @param outValue      仿真数据
             * @param frameCode     仿真码
             * @param insideCode    仿真字码
             */
            virtual void simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode = 0, const std::vector<icdInValueType> inValue = {}){}

            /**
             * @brief 协议解析，根据配置协议进行参数解析
             * @param inBuf     数据首地址
             * @param inSize    数据长度
             * @param outValue  输出数据
             */
            virtual void parse(const unsigned char *inBuf, const unsigned int inSize, std::vector<icdOutConvertValueType> &convertOutValue){}
        };

        /** 类导出函数指针 **/
        typedef frameObj *(*LOAD_FRAME_LIB)();
    }
}

#endif
