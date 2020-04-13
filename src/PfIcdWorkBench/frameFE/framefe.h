#ifndef FRAMEFE_H
#define FRAMEFE_H

#include "framefe_global.h"

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
        class FRAMEFESHARED_EXPORT frameFE: public frameObj
        {

        public:
            frameFE();
            ~frameFE();
        public:
            std::shared_ptr<frameObj> clone() override;
            void init(const TiXmlElement *ele) override;
            std::string getFrameName() override{return VAR_NAME(frameFE);}
            std::string version() override {return VERSION;}
            void simulation(byteArray &outValue, const std::string &json) override;
            void resendMsg(byteArray &outValue) override;
            bool getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &json) override;
            bool parse(unsigned char *inBuf, const unsigned int inSize, Json::Value &result) override;
        private:
            void _parseRegion(const int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue);
            void _fillRegion(byteArray &outValue, const Json::Value &jsValue);
        private:
            std::unordered_map<std::pair<unsigned int, unsigned int>, int, pair_hash> mProtocolCnt; ///< 命令计数

            const unsigned char mHeadCode = 0xFE;
        };
        extern "C"
        {
           /**
           * @brief 加载类
           * @return 类指针
           */
           FRAMEFESHARED_EXPORT frameObj *LoadClass();
        }
    }
}


#endif // FRAMEFE_H
