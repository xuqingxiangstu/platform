#include "rsa.h"

namespace Pf
{
    namespace PfCommon
    {
        bool rsa::createRsaKey(QString& strPubKey, QString& strPriKey)
        {
            RSA *pRsa = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);
            if ( !pRsa )
            {
                return false;
            }

            BIO *pPriBio = BIO_new(BIO_s_mem());
            PEM_write_bio_RSAPrivateKey(pPriBio, pRsa, NULL, NULL, 0, NULL, NULL);
            BIO *pPubBio = BIO_new(BIO_s_mem());
            PEM_write_bio_RSAPublicKey(pPubBio, pRsa);

            // 获取长度
            size_t nPriKeyLen = BIO_pending(pPriBio);
            size_t nPubKeyLen = BIO_pending(pPubBio);

            // 密钥对读取到字符串
            char* pPriKey = new char[nPriKeyLen];
            char* pPubKey = new char[nPubKeyLen];
            BIO_read(pPriBio, pPriKey, nPriKeyLen);
            BIO_read(pPubBio, pPubKey, nPubKeyLen);

            // 存储密钥对
            strPubKey = QByteArray(pPubKey, nPubKeyLen);
            strPriKey = QByteArray(pPriKey, nPriKeyLen);

            // 内存释放
            RSA_free(pRsa);
            BIO_free_all(pPriBio);
            BIO_free_all(pPubBio);

            delete pPriKey;
            delete pPubKey;

            return true;
        }

        QString rsa::rsaPriEncryptBase64(const QString& strClearData, const QString& strPriKey)
        {
            QByteArray priKeyArry = strPriKey.toUtf8();
            uchar* pPriKey = (uchar*)priKeyArry.data();
            BIO* pKeyBio = BIO_new_mem_buf(pPriKey, strPriKey.length());
            if (pKeyBio == NULL)
            {
                return "";
            }
            RSA* pRsa = RSA_new();
            pRsa = PEM_read_bio_RSAPrivateKey(pKeyBio, &pRsa, NULL, NULL);
            if ( pRsa == NULL )
            {
                BIO_free_all(pKeyBio);
                return "";
            }
            int nLen = RSA_size(pRsa);
            char* pEncryptBuf = new char[nLen];
            memset(pEncryptBuf, 0, nLen);
            QByteArray clearDataArry = strClearData.toUtf8();
            int nClearDataLen = clearDataArry.length();
            uchar* pClearData = (uchar*)clearDataArry.data();

            int nSize = RSA_private_encrypt(nClearDataLen,
            pClearData,
            (uchar*)pEncryptBuf,
            pRsa,
            RSA_PKCS1_PADDING);

            QString strEncryptData = "";
            if ( nSize >= 0 )
            {
                QByteArray arry(pEncryptBuf, nSize);
                strEncryptData = arry.toBase64();
            }
            // 释放内存
            delete pEncryptBuf;
            BIO_free_all(pKeyBio);
            RSA_free(pRsa);
            return strEncryptData;
        }

        QString rsa::rsaPubDecryptBase64(const QString& strDecryptData, const QString& strPubKey)
        {
            QByteArray pubKeyArry = strPubKey.toUtf8();
            uchar* pPubKey = (uchar*)pubKeyArry.data();
            BIO* pKeyBio = BIO_new_mem_buf(pPubKey, strPubKey.length());
            if (pKeyBio == NULL)
            {
                return "";
            }
            RSA* pRsa = RSA_new();
            if ( strPubKey.contains(BEGIN_RSA_PUBLIC_KEY) )
            {
                pRsa = PEM_read_bio_RSAPublicKey(pKeyBio, &pRsa, NULL, NULL);
            }
            else
            {
                pRsa = PEM_read_bio_RSA_PUBKEY(pKeyBio, &pRsa, NULL, NULL);
            }
            if ( pRsa == NULL )
            {
                BIO_free_all(pKeyBio);
                return "";
            }
            int nLen = RSA_size(pRsa);
            char* pClearBuf = new char[nLen];
            memset(pClearBuf, 0, nLen);
            //解密
            QByteArray decryptDataArry = strDecryptData.toUtf8();
            decryptDataArry = QByteArray::fromBase64(decryptDataArry);
            int nDecryptDataLen = decryptDataArry.length();
            uchar* pDecryptData = (uchar*)decryptDataArry.data();
            int nSize = RSA_public_decrypt(nDecryptDataLen,
            pDecryptData,
            (uchar*)pClearBuf,
            pRsa,
            RSA_PKCS1_PADDING);
            QString strClearData = "";
            if ( nSize >= 0 )
            {
                strClearData = QByteArray(pClearBuf, nSize);
            }
            // 释放内存
            delete pClearBuf;
            BIO_free_all(pKeyBio);
            RSA_free(pRsa);
            return strClearData;
        }

        QString rsa::rsaPubEncryptBase64(const QString& strClearData, const QString& strPubKey)
        {
            QByteArray pubKeyArry = strPubKey.toUtf8();
            uchar* pPubKey = (uchar*)pubKeyArry.data();
            BIO* pKeyBio = BIO_new_mem_buf(pPubKey, pubKeyArry.length());
            if (pKeyBio == NULL)
            {
                return "";
            }
            RSA* pRsa = RSA_new();
            if ( strPubKey.contains(BEGIN_RSA_PUBLIC_KEY) )
            {
                pRsa = PEM_read_bio_RSAPublicKey(pKeyBio, &pRsa, NULL, NULL);
            }
            else
            {
                pRsa = PEM_read_bio_RSA_PUBKEY(pKeyBio, &pRsa, NULL, NULL);
            }
            if ( pRsa == NULL )
            {
                BIO_free_all(pKeyBio);
                return "";
            }
            int nLen = RSA_size(pRsa);
            char* pEncryptBuf = new char[nLen];
            memset(pEncryptBuf, 0, nLen);
            QByteArray clearDataArry = strClearData.toUtf8();
            int nClearDataLen = clearDataArry.length();
            uchar* pClearData = (uchar*)clearDataArry.data();

            int nSize = RSA_public_encrypt(nClearDataLen,
            pClearData,
            (uchar*)pEncryptBuf,
            pRsa,
            RSA_PKCS1_PADDING);

            QString strEncryptData = "";
            if ( nSize >= 0 ){
                QByteArray arry(pEncryptBuf, nSize);
                strEncryptData = arry.toBase64();
            }
            // 释放内存
            delete pEncryptBuf;
            BIO_free_all(pKeyBio);
            RSA_free(pRsa);
            return strEncryptData;
        }

        QString rsa::rsaPriDecryptBase64(const QString& strDecryptData, const QString& strPriKey)
        {
            QByteArray priKeyArry = strPriKey.toUtf8();
            uchar* pPriKey = (uchar*)priKeyArry.data();
            BIO* pKeyBio = BIO_new_mem_buf(pPriKey, priKeyArry.length());
            if (pKeyBio == NULL)
            {
                return "";
            }
            RSA* pRsa = RSA_new();
            pRsa = PEM_read_bio_RSAPrivateKey(pKeyBio, &pRsa, NULL, NULL);
            if ( pRsa == NULL )
            {
                BIO_free_all(pKeyBio);
                return "";
            }
            int nLen = RSA_size(pRsa);
            char* pClearBuf = new char[nLen];
            memset(pClearBuf, 0, nLen);
            //解密
            QByteArray decryptDataArry = strDecryptData.toUtf8();
            decryptDataArry = QByteArray::fromBase64(decryptDataArry);
            int nDecryptDataLen = decryptDataArry.length();
              uchar* pDecryptData = (uchar*)decryptDataArry.data();
              int nSize = RSA_private_decrypt(nDecryptDataLen,
            pDecryptData,
            (uchar*)pClearBuf,
            pRsa,
            RSA_PKCS1_PADDING);

            QString strClearData = "";
            if ( nSize >= 0 )
            {
                strClearData = QByteArray(pClearBuf, nSize);
            }
            // 释放内存
            delete pClearBuf;
            BIO_free_all(pKeyBio);
            RSA_free(pRsa);
            return strClearData;
        }
    }
}
