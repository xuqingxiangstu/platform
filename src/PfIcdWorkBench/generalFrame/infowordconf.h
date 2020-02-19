#ifndef INFOWORDCONF_H
#define INFOWORDCONF_H

/**
 * @file infoWordConf.h
 * @brief 信息字配置信息
 * @author xqx
 * @date 20200219
 * @version V1.0.0
 */

#include <xlsxdocument.h>
#include <xlsxworksheet.h>
#include <xlsxworkbook.h>

#include <memory>

#define DEBUG_ICD 0

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The infoConf class
         * 信息字配置基类
         */
        class infoConf
        {
        public:
            /**
             * @brief getInfoLen    获取信息字长度
             * @param[in] msg 帧首地址
             * @param[in] msgSize 帧长度
             * @param[out] len 信息字长度
             * @return 是否成功
             * - true:成功
             * - false:失败
             */
            virtual bool getInfoLen(const unsigned char *msg, const int &msgSize, int &len){return false;}

            /**
             * @brief init  初始化信息字配置
             * @param sheet Excel表格Sheet
             */
            virtual void init(QXlsx::Worksheet *sheet){}

            /**
             * @brief getInfoType   获取信息字类型
             * @return 类型
             */
            virtual int getInfoType() { return -1; }

            /**
             * @brief clone 类克隆，深拷贝
             * @return 类句柄
             */
            virtual std::shared_ptr<infoConf> clone(){return nullptr;}
        };

        /**
         * @brief The infoWord1Conf class
         * 信息字1配置
         */

        class infoWord1Conf : public infoConf
        {
        public:
            infoWord1Conf();
            ~infoWord1Conf(){}
        public:
            void init(QXlsx::Worksheet *sheet) override;
            bool getInfoLen(const unsigned char *msg, const int &msgSize, int &len) override;
            int getInfoType() override {return mInfoType;}
            std::shared_ptr<infoConf> clone() override;
        private:
            int mInfoLen;
            int mInfoType;
        };

        /**
         * @brief The infoWord2Conf class
         * 信息字2配置
         */

        class infoWord2Conf : public infoConf
        {
        public:
            infoWord2Conf();
            ~infoWord2Conf(){}
        public:
            void init(QXlsx::Worksheet *sheet) override;
            bool getInfoLen(const unsigned char *msg, const int &msgSize, int &len) override;
            int getInfoType() override {return mInfoType;}
            std::shared_ptr<infoConf> clone() override;
        private:
            int mInfoLen;
            int mInfoType;
        };
    }
}
#endif // INFOWORDCONF_H
