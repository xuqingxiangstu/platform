#ifndef SUBPROTOCOLCONFIGURE_H
#define SUBPROTOCOLCONFIGURE_H

/**
 * @brief The protocolConfigure class
 * 子帧配置类（数据域配置信息）
 * @author xqx
 * @date 20200210
 * @version 1.0
 */

#include <unordered_map>

#include "../common/subProtocol.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        class subProtocolConfigure : public subProtocol
        {
        public:
            subProtocolConfigure();
            ~subProtocolConfigure();
        public:
            void init(QXlsx::Workbook *) override;
            std::string curProtocol() override;
            bool isExist(unsigned int frameCode, unsigned int insideCode = 0) override;
            int getMsgLen(unsigned int frameCode, unsigned int insideCode = 0) override;
            int getMsgBeyond(unsigned int frameCode, unsigned int insideCode = 0) override;
            int getMaxByteSize(unsigned int frameCode, unsigned int insideCode = 0) override;

            /**
             * @brief getKeys   获取子帧关键字集合
             * @param keys      关键字集合
             */
            void getKeys(std::vector<std::pair<unsigned int, unsigned int>> &keys);

            /**
             * @brief getStorages   根据帧识别码、子帧识别码获取子帧信息
             * @param frameCode     帧识别码
             * @param insideCode    子帧识别码
             * @return true/false 是否获取成功
             */
            bool getStorages(std::vector<std::shared_ptr<subStorageType>> &, unsigned int frameCode, unsigned int insideCode = 0);
        private:

            /// 子帧信息校验
            void checkParamValid(std::string, int, int, std::string, std::string, int, int, int, int, std::string, int, int, int, int);
        private:
            std::unordered_map<subMapKeyType, subMapValueType, pair_hash> mParamsInfo;  ///< 子帧信息存储
        };
    }
}

#endif // SUBPROTOCOLCONFIGURE_H
