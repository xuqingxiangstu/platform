#ifndef FRAMEDM_H
#define FRAMEDM_H

#include "framedm_global.h"

#include "../common/frameObj.h"
#include "../../PfCommon/jsoncpp/json.h"

#include <memory>
#include <unordered_map>

#define VERSION "1.0.1"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        class FRAMEDMSHARED_EXPORT frameDM : public frameObj
        {

        public:
            frameDM();
            ~frameDM();
        public:
            std::shared_ptr<frameObj> clone() override;
            void init(const TiXmlElement *ele) override;
            std::string getFrameName() override{return VAR_NAME(frameDM);}
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
           FRAMEDMSHARED_EXPORT frameObj *LoadClass();
        }
    }
}
#endif // FRAMEDM_H
