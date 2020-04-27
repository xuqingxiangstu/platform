#ifndef BUSINESSADAPTER_H
#define BUSINESSADAPTER_H

/**
 * @brief The businessAdapter class 业务相关适配
 */

#include <memory>

#include "business.h"


class businessAdapter
{
public:
    businessAdapter();

    static std::shared_ptr<businessAdapter> getInstance()
    {
        if(mInstance.get() == nullptr)
        {
            mInstance = std::make_shared<businessAdapter>();
        }
        return mInstance;
    }

    /**
     * @brief getBusiness   获取业务逻辑
     * @param ptl           协议
     * @return
     */
    std::shared_ptr<business> getBusiness(const std::string &ptl);

private:
    static std::shared_ptr<businessAdapter> mInstance;
    std::unordered_map<std::string, std::shared_ptr<business>> mBusinessManagement; ///< 业务管理适
};

#endif // BUSINESSADAPTER_H
