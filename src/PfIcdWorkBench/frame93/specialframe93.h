#ifndef SPECIALFRAME93_H
#define SPECIALFRAME93_H

#include "frame93_global.h"

#include "../common/frameObj.h"
#include "../../PfCommon/jsoncpp/json.h"

#include <memory>
#include <unordered_map>

#define VERSION "1.0.1"

/* 校验类型 0：CRC；1：累加和 */
#define CHECK_CRC 0

/* CRC字节数 */
#define CRC_SIZE 1

//#define CHECK "SUM"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The specialFrame93 class
         * 特殊帧93操作：解析、仿真等
         */
        class FRAME93SHARED_EXPORT specialFrame93: public frameObj
        {

        public:
            specialFrame93();
            ~specialFrame93();
        public:
            std::shared_ptr<frameObj> clone() override;
            void init(const TiXmlElement *ele) override;
            std::string getFrameName() override{return VAR_NAME(specialFrame93);}
            std::string version() override {return VERSION;}
            void simulation(byteArray &outValue, const Json::Value &json) override;
            void simulation(byteArray &outValue, const std::string &json) override;
            bool getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &json) override;
            bool parse(unsigned char *inBuf, const unsigned int inSize, Json::Value &result) override;
            void setUuid(const std::string &uuid) override {mCurUuid = uuid;}
        private:
            void _parseRegion(const unsigned int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue);
            void _fillRegion(byteArray &outValue, const Json::Value &jsValue);
        private:
            std::string mCurUuid;
        };
        extern "C"
        {
           /**
           * @brief 加载类
           * @return 类指针
           */
           FRAME93SHARED_EXPORT frameObj *LoadClass();
        }
    }
}

#endif // SPECIALFRAME93_H
