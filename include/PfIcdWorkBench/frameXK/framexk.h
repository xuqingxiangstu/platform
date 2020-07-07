#ifndef FRAMEXK_H
#define FRAMEXK_H

#include "framexk_global.h"

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
        class FRAMEXKSHARED_EXPORT frameXK: public frameObj
        {

        public:
            frameXK();
            ~frameXK();
        public:
            std::shared_ptr<frameObj> clone() override;
            void init(const TiXmlElement *ele) override;
            std::string getFrameName() override{return VAR_NAME(frameXK);}
            std::string version() override {return VERSION;}
            void simulation(byteArray &outValue, const Json::Value &json) override;
            void simulation(byteArray &outValue, const std::string &json) override;
            void resendMsg(byteArray &outValue) override;
            bool parse(unsigned char *inBuf, const unsigned int inSize, Json::Value &result) override;
        private:
            void _fillRegion(byteArray &outValue, const Json::Value &regionJs);
        };
        extern "C"
        {
           /**
           * @brief 加载类
           * @return 类指针
           */
           FRAMEXKSHARED_EXPORT frameObj *LoadClass();
        }
    }
}

#endif // FRAMEXK_H
