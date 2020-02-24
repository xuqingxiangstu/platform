#ifndef PROTOCOLSTORAGE_H
#define PROTOCOLSTORAGE_H

/**
 * @file protocolstorage.h
 * @brief 协议存储类
 * @author xqx
 * @date 2020-02-10
 * @version V1.0
 */

#include <tuple>
#include <memory>

#include <xlsxdocument.h>
#include <xlsxworksheet.h>
#include <xlsxworkbook.h>


namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief protocolStorage
         * 协议存储
         */
        template<typename... Args>
        class protocolStorage
        {
        public:
            ~protocolStorage()
            {

            }

            /**
             * @brief protocolStorage
             * 构造函数
             * @param arg tuple参数
             */
            protocolStorage(Args... arg)
            {
                mMessage = std::make_tuple(arg...);
            }

            /**
             * @brief   getMessage  获取相应消息
             * @param   index       消息索引
             * @return  消息内容
             */
            template<size_t index>
            auto getMessage()
            {
                return std::get<index>(mMessage);
            }

            /**
             * @brief clone 类克隆
             * @return  类句柄
             */
            auto clone()
            {
                std::shared_ptr<protocolStorage<Args...>> tmpShare(new protocolStorage<Args...>());
                tmpShare->mMessage = this->mMessage;
                return tmpShare;
            }

        private:
            protocolStorage()
            {

            }


        private:
            std::tuple<Args...> mMessage; ///< 数据存储元祖
        };
    }
}
#endif // PROTOCOLSTORAGE_H
