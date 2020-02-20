#ifndef GENERALFRAME_H
#define GENERALFRAME_H

/**
 * @file generalFrame.h
 * @brief 通用操作类
 * @author xqx
 * @date 2020-02-10
 * @copyright (C)2020 BJTU
 */

#include "generalframe_global.h"
#include "../common/frameObj.h"
#include "protocolconfigure.h"
#include "infowordconf.h"
#include "infowordregion.h"

#include <memory>
#include <unordered_map>

#define VERSION "1.0.1"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The variableFrame class
         * 通用帧操作：解析、仿真等
         */
        class GENERALFRAMESHARED_EXPORT generalFrame : public frameObj
        {

        public:
            generalFrame();
            ~generalFrame();
        public:
            std::shared_ptr<frameObj> clone() override;
            void init(const TiXmlElement *ele) override;
            std::string getFrameName() override{return VAR_NAME(variableFrame);}
            std::string version() override {return VERSION;}
            void simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode = 0, const std::vector<icdInValueType> inValue = {}) override;
            void parse(const unsigned char *inBuf, const unsigned int inSize, std::vector<icdOutConvertValueType> &convertOutValue) override;

        private:
            void _simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode = 0, const std::vector<icdInValueType> inValue = {});
            /**
             * @brief frameCheck    帧校验
             * @param[in] inBuf         校验首地址
             * @param[in] inSize        校验长度
             */
            void frameCheck(const unsigned char *inBuf, const unsigned int inSize);

            /**
             * @brief upDataCrc 更新crc
             * @param[in] u8Msg     数据首地址
             * @param[in] u32Size   数据长度
             */
            void upDataCrc(unsigned char *u8Msg, const unsigned int u32Size);

            void _parse(const unsigned);
        private:
            /**
             * @brief initFrameCfg  初始化帧配置(Excel)
             * @param[in] path 帧配置路径
             */
            void initFrameCfg(const std::string &path);

            /**
             * @brief initSubFrameCfg   初始化子帧配置（excel）
             * @param[in] path 子帧配置路径
             */
            void initSubFrameCfg(const std::string &path);
        private:
            std::string mFrameCfgPath;  ///< 帧配置路径
            std::string mDataRegionCfgPath; ///< 数据域配置路径
            std::shared_ptr<protocolConfigure> mProtocolCfg; ///< 帧配置
            std::shared_ptr<infoWordRegionManager> mSubProtocolCfg; ///< 子帧配置（数据域描述）
            std::unordered_map<std::pair<unsigned int, unsigned int>, int, pair_hash> mProtocolCnt; ///< 命令计数
            std::unordered_map<unsigned int, std::shared_ptr<infoConf>> mInfoWordConf; ///< 信息字配置
        };

        extern "C"
        {
            /**
            * @brief 加载类
            * @return 类指针
            */
            GENERALFRAMESHARED_EXPORT frameObj *LoadClass();
        }
    }
}

#endif // GENERALFRAME_H
