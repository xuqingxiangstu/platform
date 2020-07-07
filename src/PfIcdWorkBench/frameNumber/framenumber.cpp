#include "framenumber.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        std::shared_ptr<frameNumberManager> frameNumberManager::mInstance = nullptr;

        frameNumberManager::frameNumberManager()
        {

        }

        frameNumberManager::~frameNumberManager()
        {

        }

        int frameNumberManager::getFrameNumber(const std::string &dev, const std::string &ptl, unsigned int srcNode, unsigned int dstNode)
        {
            int number = 0;

            std::string key = dev + "_" + ptl;

            auto itor = mProtocolCnt.find(key);
            if(itor != mProtocolCnt.end())//找到后返回并累加
            {
                mapValue value = itor->second;
                value[std::make_pair(srcNode, dstNode)] = value[std::make_pair(srcNode, dstNode)] + 1;

                mMutex.lock();
                mProtocolCnt[key] = value;
                mMutex.unlock();

                number = value[std::make_pair(srcNode, dstNode)];
            }
            else//初始化
            {
                mapValue value;
                value[std::make_pair(srcNode, dstNode)] = 0;

                mMutex.lock();
                mProtocolCnt[key] = value;
                mMutex.unlock();

                number = 0;
            }

            return number;
        }
    }
}
