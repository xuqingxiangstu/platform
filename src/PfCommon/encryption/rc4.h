#ifndef RC4_H
#define RC4_H


#include "./openssl/include/openssl/aes.h"
#include "./openssl/include/openssl/rc4.h"
#include <QString>

namespace Pf
{
    namespace PfCommon
    {
        /**
         * @brief The aes class RC4加解密
         */
        class rc4
        {
        public:
            /**
             * @brief file_encrypt  文件加密(
             * @param clearfile     明文文件
             * @param key           秘钥
             * @return              是否成功
             */
            static bool file_encrypt(const QString &clearfile, const QString &key);

            /**
             * @brief file_decrypt  文件解密
             * @param encryptfile   加密文件
             * @param key           秘钥
             * @return              是否成功
             */
            static bool file_decrypt(const QString &encryptfile, const QByteArray &key);

            /**
             * @brief data_encrypt  数据加密
             * @param clearData     明文
             * @param decryptData   密文
             * @param key           秘钥
             */
            static void data_encrypt(const QByteArray &clearData, QByteArray &decryptData, const QString &key);

            /**
             * @brief data_decrypt  数据解密
             * @param decryptData   密文
             * @param clearData     明文
             * @param key           秘钥
             */
            static void data_decrypt(const QByteArray &decryptData, QByteArray &clearData, const QString &key);
        };
    }
}

#endif // RC4_H
