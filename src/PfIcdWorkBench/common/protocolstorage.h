#ifndef PROTOCOLSTORAGE_H
#define PROTOCOLSTORAGE_H

/** 协议存储类
 *  @brief The protocolStorage class
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

        template<typename... Args>
        class protocolStorage
        {
        public:
            ~protocolStorage()
            {

            }

            /**
             * @brief protocolStorage
             * @param arg
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
            /** 数据存储元组
             * @brief mMessage
             */
            std::tuple<Args...> mMessage;
        };
    }
}
#endif // PROTOCOLSTORAGE_H
