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
#include "../../PfCommon/jsoncpp/json.h"
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
             * @param[out] outValue      仿真数据
             * @param[in] frameCode     仿真码
             * @param[in] insideCode    仿真字码
             * @return
             */
            virtual void simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode = 0, const std::vector<icdInValueType> inValue = {}){}

            /**
             * @brief simulation    协议仿真
             * @param outValue 仿真数据
             * @param json json协议             
             */
            virtual void simulation(byteArray &outValue, const std::string &json){}


            virtual void simulation(byteArray &outValue, const Json::Value &json){}
            /**
             * @brief simulation    协议仿真
             * @param outValue 仿真数据
             * @param json json协议
             */
            virtual void simulation(byteArray &outValue, const byteArray &inValue){}

            /**
             * @brief getAskMsg 获取应答帧
             * @param outValue  应答数据
             * @param json  协议
             * @return
             */
            virtual bool getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &json){return false;}

            /**
             * @brief getValidValue 获取有效值
             * @param json  值
             * @param infoType  信息字类型
             * @return
             */
            virtual bool getValidValue(const Json::Value &result, Json::Value &value, int &infoType){return false;}

            /**
             * @brief resendMsg 重传消息，只改变重传次数
             * @param outValue  数据
             */
            virtual void resendMsg(byteArray &outValue){}

            /**
             * @brief 协议解析，根据配置协议进行参数解析
             * @param[in] inBuf     数据首地址
             * @param[in] inSize    数据长度
             * @param[out] outValue  输出数据
             * @return
             */
            virtual void parse(const unsigned char *inBuf, const unsigned int inSize, std::vector<icdOutConvertValueType> &convertOutValue){}

            /**
             * @brief parse 协议解析，根据配置协议进行参数解析
             * @param[in] inBuf  数据首地址
             * @param[in] inSize 数据长度
             * @return json帧
             */
            virtual bool parse(unsigned char *inBuf, const unsigned int inSize, Json::Value &value){return false;}

            /**
             * @brief setAttribute  设置属性
             * @param attr  属性
             */
            virtual void setAttribute(const Json::Value &attr){}

            /**
             * @brief setUuid   设置UUID
             * @param uuid
             */
            virtual void setUuid(const std::string &uuid){}
        };

        /** 类导出函数指针 **/
        typedef frameObj *(*LOAD_FRAME_LIB)();
    }
}

#endif
