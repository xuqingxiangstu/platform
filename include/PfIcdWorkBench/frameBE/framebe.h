#ifndef FRAMEBE_H
#define FRAMEBE_H

#include "framebe_global.h"

#include "../common/frameObj.h"
#include "../../PfCommon/jsoncpp/json.h"

#include <memory>
#include <unordered_map>

#define VERSION "1.0.1"

#define DEBUG_MIDDLE  0

namespace Pf
{
    namespace PfIcdWorkBench
    {
        class FRAMEBESHARED_EXPORT frameBE : public frameObj
        {
            enum{
                Info_Word_0,
                Info_Word_1,
                Info_Word_2
            };
        public:
            frameBE(){}
            ~frameBE(){}
        public:
            std::shared_ptr<frameObj> clone() override;
            void init(const TiXmlElement *ele) override;
            std::string getFrameName() override{return VAR_NAME(frameMiddle);}
            std::string version() override {return VERSION;}
            void simulation(byteArray &outValue, const std::string &json) override;
            void resendMsg(byteArray &outValue) override;
            bool getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &json) override;
            bool parse(unsigned char *inBuf, const unsigned int inSize, Json::Value &result) override;
            bool getValidValue(const Json::Value &result, Json::Value &value, int &infoType) override;
            void setUuid(const std::string &uuid) override {mCurUuid = uuid;}
        private:
            /**
             * @brief frameCheck    帧校验
             * @param[in] inBuf         校验首地址
             * @param[in] inSize        校验长度
             */
            void frameCheck(unsigned char *inBuf, const unsigned int inSize);
            /**
             * @brief getMiddleInfoWordIndex    获取中间件信息字类型
             * @param inBuf
             * @param inSize
             * @return
             */
            int getInfoWordIndex(unsigned char *inBuf, const unsigned int inSize);

            int getInfoWordSizeIndex(unsigned char *inBuf, const unsigned int inSize);
            int getInfoWordSize(unsigned char *inBuf, const unsigned int inSize);

            int getHeadSize(unsigned char *inBuf, const unsigned int inSize);

            bool getInfoLen(unsigned char *inBuf, const unsigned int inSize, int infoType, int &len);

        private:
            void _parseHead(unsigned char *inBuf, const unsigned int inSize, Json::Value &result);
            void _parseRegion(const int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue);
            /**
             * @brief _parseInfo    解析信息字
             * @param[in] infoType  信息字类型
             * @param[in] inBuf 源码首地址
             * @param[in] inSize 源码长度
             */
            void _parseInfo(int infoType, const unsigned char *inBuf, const unsigned int inSize, Json::Value &value, Json::Value &regionJs, int tableNum);
            void _parseInfoWord0(const unsigned char *inBuf, const unsigned int inSize, Json::Value &values);
            void _parseInfoWord1(const unsigned char *inBuf, const unsigned int inSize, Json::Value &value, Json::Value &regionJs, int tableNum);
            void _parseInfoWord2(const unsigned char *inBuf, const unsigned int inSize, Json::Value &value);
            void _fillRegion(byteArray &outValue, const Json::Value &jsValue);

            void fillHead(byteArray &outValue, const std::vector<byteArray> &wordMsg, Json::Value jsValue);

            void _fillInfo(int infoType, byteArray &outValue, Json::Value headJs, Json::Value wordsJs, Json::Value regionJs);

            void _fillInfoWord0(byteArray &outValue, Json::Value wordsJs);
            void _fillInfoWord1(byteArray &outValue, Json::Value headJs, Json::Value wordsJs, Json::Value regionJs);
            void _fillInfoWord2(byteArray &outValue, Json::Value wordsJs);
        private:
            std::string mCurUuid;
            const unsigned char mHeadCode = 0xBE;
        };

        extern "C"
        {
           /**
           * @brief 加载类
           * @return 类指针
           */
           FRAMEBESHARED_EXPORT frameObj *LoadClass();
        }
    }
}

#endif // FRAMEBE_H
