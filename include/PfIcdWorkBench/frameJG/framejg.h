#ifndef FRAMEJG_H
#define FRAMEJG_H

#include "framejg_global.h"

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
        class FRAMEJGSHARED_EXPORT frameJG: public frameObj
        {

        public:
            frameJG();
            ~frameJG();
        public:
            std::shared_ptr<frameObj> clone() override;
            void init(const TiXmlElement *ele) override;
            std::string getFrameName() override{return VAR_NAME(frameJG);}
            std::string version() override {return VERSION;}
            void simulation(byteArray &outValue, const Json::Value &json) override;
            void simulation(byteArray &outValue, const std::string &json) override;
            void resendMsg(byteArray &outValue) override;
            bool parse(unsigned char *inBuf, const unsigned int inSize, Json::Value &result) override;
            void setUuid(const std::string &uuid) override {mCurUuid = uuid;}
        private:
            void _fillRegion(byteArray &outValue, const Json::Value &regionJs);
            void _parseRegion(const unsigned int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue);
        private:
            std::string mCurUuid;
            std::unordered_map<std::pair<unsigned int, unsigned int>, int, pair_hash> mProtocolCnt; ///< 命令计数
        };
        extern "C"
        {
           /**
           * @brief 加载类
           * @return 类指针
           */
           FRAMEJGSHARED_EXPORT frameObj *LoadClass();
        }
    }
}

#endif // FRAMEJG_H
