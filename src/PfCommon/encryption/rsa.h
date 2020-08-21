#ifndef RSA_H
#define RSA_H


/**
 * @file    rsaencryption.h
 * @brief   RSA加解密接口，可生成公钥私钥对，利用公钥加密，私钥解密进行安全数据传输
 * @author  xqx
 * @date    2020-8-11 10:14:36
 * @version v1.0
 */

#include "./openssl/include/openssl/rsa.h"
#include "./openssl/include/openssl/pem.h"
#include "./openssl/include/openssl/err.h"

#include "encryption_global.h"

#include <QString>
#include <QByteArray>

#define BEGIN_RSA_PUBLIC_KEY    "BEGIN RSA PUBLIC KEY"
#define BEGIN_PUBLIC_KEY        "BEGIN PUBLIC KEY"
#define KEY_LENGTH 1024         // 密钥长度

namespace Pf
{
    namespace PfCommon
    {
        /**
         * @brief The rsaEncryption class   加解密及秘钥对生成
         */
        class ENCRYPTIONSHARED_EXPORT rsa
        {
        public:
            /**
            * @brief createRsaKey 生成秘钥对
            * @param strPubKey 公钥
            * @param strPriKey 私钥
            * @return 状态
            */
            static bool createRsaKey (QString& strPubKey, QString& strPriKey);
            /**
            * @brief rsaPriEncryptBase64 私钥加密
            * @param strClearData 明文
            * @param strPriKey 私钥
            * @return 加密后数据(base64格式)
            */
            static QString rsaPriEncryptBase64(const QString& strClearData, const QString& strPriKey);
            /**
            * @brief rsaPubDecryptBase64 公钥解密
            * @param strDecrypt 待解密数据(base64格式)
            * @param strPubKey 公钥钥
            * @return 明文
            */
            static QString rsaPubDecryptBase64 (const QString& strDecryptData, const QString& strPubKey);
            /**
            * @brief rsaPubEncryptBase64 公钥加密
            * @param strClearData 明文
            * @param strPubKey 私钥
            * @return 加密后数据(base64格式)
            */
            static QString rsaPubEncryptBase64 (const QString& strClearData, const QString& strPubKey);
            /**
            * @brief rsaPriDecryptBase64 私钥解密
            * @param strDecrypt 待解密数据(base64格式)
            * @param strPriKey 私钥
            * @return 明文
            */
            static QString rsaPriDecryptBase64 (const QString& strDecryptData, const QString& strPriKey);
        };
    }
}
#endif // RSA_H
