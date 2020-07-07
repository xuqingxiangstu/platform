#ifndef FRAMENUMBER_H
#define FRAMENUMBER_H

#include "framenumber_global.h"
#include <memory>
#include <mutex>
#include <unordered_map>

namespace Pf
{
    namespace PfIcdWorkBench
    {
        class FRAMENUMBERSHARED_EXPORT frameNumberManager
        {
            /**
             * @brief The pair_hash struct
             * unordered_map 自定义hasher
             */
            struct pair_hash
            {
                template<class T1, class T2>
                std::size_t operator() (const std::pair<T1, T2>& p) const
                {
                    auto h1 = std::hash<T1>{}(p.first);
                    auto h2 = std::hash<T2>{}(p.second);
                    return h1 == h2;
                }
            };
        public:
            static std::shared_ptr<frameNumberManager> getInstance()
            {
                if(mInstance == nullptr)
                {
                    mInstance = std::make_shared<frameNumberManager>();
                }
                return mInstance;
            }
        public:
            frameNumberManager();
            ~frameNumberManager();

        public:
            /**
             * @brief getFrameNumber    获取命令计数
             * @param dev       设备
             * @param ptl       协议
             * @param srcNode   源节点
             * @param dstNode   目标节点
             * @return
             */
            int getFrameNumber(const std::string &dev, const std::string &ptl, unsigned int srcNode, unsigned int dstNode);
        private:
            std::mutex mMutex;
            using mapValue = std::unordered_map<std::pair<unsigned int, unsigned int>, int, pair_hash>;
            std::unordered_map<std::string, mapValue> mProtocolCnt; ///< 命令计数
            static std::shared_ptr<frameNumberManager> mInstance;
        };
    }
}

#endif // FRAMENUMBER_H
