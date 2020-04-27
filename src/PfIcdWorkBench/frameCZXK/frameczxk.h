#ifndef FRAMECZXK_H
#define FRAMECZXK_H

#include "frameczxk_global.h"

class FRAMECZXKSHARED_EXPORT frameCZXK
{

public:
    frameCZXK();
};

#include "../common/frameObj.h"
#include "../../PfCommon/jsoncpp/json.h"

#include <memory>
#include <unordered_map>

#define VERSION "1.0.1"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The specialFrame93 class
         * 特殊帧93操作：解析、仿真等
         */
        class FRAMECZXKSHARED_EXPORT frameCZXK: public frameObj
        {

        public:
            frameCZXK();
            ~frameCZXK();
        public:
            std::shared_ptr<frameObj> clone() override;
            void init(const TiXmlElement *ele) override;
            std::string getFrameName() override{return VAR_NAME(frameCZXK);}
            std::string version() override {return VERSION;}
            void simulation(byteArray &outValue, const Json::Value &json) override;
            void simulation(byteArray &outValue, const std::string &json) override;
            void resendMsg(byteArray &outValue) override;
            bool parse(unsigned char *inBuf, const unsigned int inSize, Json::Value &result) override;
        private:
            void _fillRegion(byteArray &outValue, const Json::Value &regionJs);
            void _parseRegion(const unsigned int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue);
        private:
            std::unordered_map<std::pair<unsigned int, unsigned int>, int, pair_hash> mProtocolCnt; ///< 命令计数
        };
        extern "C"
        {
           /**
           * @brief 加载类
           * @return 类指针
           */
           FRAMECZXKSHARED_EXPORT frameObj *LoadClass();
        }
    }
}

#endif // FRAMECZXK_H
