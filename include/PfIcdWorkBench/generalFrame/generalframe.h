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

#include "../../PfCommon/jsoncpp/json.h"

#include <memory>
#include <unordered_map>

#define VERSION "1.0.1"

#define FRAME_BE    "BE"
#define FRAME_MIDDLE    "中间件"

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
            /** 帧类型  **/
            enum frameType{
                Frame_BE,
                Frame_Middle
            };

        public:
            generalFrame();
            ~generalFrame();
        public:
            void setAttribute(const Json::Value &attr) override;
            std::shared_ptr<frameObj> clone() override;
            void init(const TiXmlElement *ele) override;
            std::string getFrameName() override{return VAR_NAME(variableFrame);}
            std::string version() override {return VERSION;}
            bool getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &json) override;
            void simulation(byteArray &outValue, const std::string &json) override;
            //void simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode = 0, const std::vector<icdInValueType> inValue = {}) override;
            bool parse(unsigned char *inBuf, const unsigned int inSize, Json::Value &result) override;
            void resendMsg(byteArray &outValue) override;
            bool getValidValue(const Json::Value &result, Json::Value &value) override;
        private:
            /**
             * @brief getMiddleInfoWordIndex    获取中间件信息字类型
             * @param inBuf
             * @param inSize
             * @return
             */
            int getMiddleInfoWordIndex(unsigned char *inBuf, const unsigned int inSize);

            int getMiddleInfoWordSize(unsigned char *inBuf, const unsigned int inSize);

            int getMiddleHeadSize(unsigned char *inBuf, const unsigned int inSize);

            /**
             * @brief getMiddleOtherHeadInfo    获取中间件其它头信息
             * @param inBuf     首地址
             * @param inSize    长度
             * @param headInfo  头信息
             */
            void getMiddleOtherHeadInfo(unsigned char *inBuf, const unsigned int inSize, Json::Value &headInfo);

            bool beAskMsg(byteArray &outValue, const Json::Value &json);
            bool middleAskMsg(byteArray &outValue, const Json::Value &json);

            void fillMiddleHead(byteArray &outValue, Json::Value &info, Json::Value jsValue);
            /**
             * @brief fillRegion    BE帧格式2域填充
             * @param[out] outValue  输出值
             * @param[in] jsValue  输入
             */
            void fillRegion(byteArray &outValue, const Json::Value &jsValue);
            void fillData(byteArray &outValue, infoWordRegion *region, infoConf *conf, const Json::Value &jsValue);
            /**
             * @brief getHeadAndWord    获取帧头及信息字
             * @param json json字符串
             * @param head 帧头
             * @param words 信息字
             */
            void getHeadAndWord(const std::string &json, Json::Value &head, Json::Value &words, Json::Value &regionJson, Json::Value &resendJson);
            /**
             * @brief _simFrame  全帧仿真
             * @param outValue  仿真数据
             * @param frameType 帧类型
             * @param infoWordType 信息字类型
             * @param wordsValue 信息字内容
             */
            void _simFrame(byteArray &outValue, const byteArray &headValue, const std::vector<byteArray> &wordsValue, const byteArray &regionValue, const int &resendCnt, const int &ack, const int &tableNum, const Json::Value &middleJs);



            //void _simulation(const infoWordRegion *region, byteArray &outValue, const std::vector<icdInValueType> inValue = {});
            /**
             * @brief frameCheck    帧校验
             * @param[in] inBuf         校验首地址
             * @param[in] inSize        校验长度
             */
            void frameCheck(unsigned char *inBuf, const unsigned int inSize);

            /**
             * @brief upDataCrc 更新crc
             * @param[in] u8Msg     数据首地址
             * @param[in] u32Size   数据长度
             */
            void upDataCrc(unsigned char *u8Msg, const unsigned int u32Size);

            /**
             * @brief _parseInfo    解析信息字
             * @param[in] region  信息域内容
             * @param[in] inBuf 源码首地址
             * @param[in] inSize 源码长度
             */
            void _parseInfo(const infoWordRegion *region, const unsigned char *inBuf, const unsigned int inSize, Json::Value &value);

            void _parseRegion(const Json::Value &wordJsons, const unsigned char *inBuf, const unsigned int inSize, Json::Value &value);
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
            const unsigned int headCode = 0xFF; ///< 信息头识别吗
            frameType mCurFrameType;    ///< 当前帧类型
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
