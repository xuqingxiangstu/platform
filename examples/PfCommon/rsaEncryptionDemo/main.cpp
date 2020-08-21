#include <QCoreApplication>
#include <QDebug>

/**
 * @brief   RSA加解密算法例程
 * @example PfCommon/rsaEncryptionDemo/main.cpp
 */

#include "../../../src/platform.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //创建秘钥对
    QString privateKey = "";
    QString publicKey = "";

    Pf::PfCommon::rsaEncryption::createRsaKey(publicKey, privateKey);

    qDebug() << publicKey << "\n" << privateKey;

    //利用公钥加密

    QString clearV = "hello word";

    QString encryptV = Pf::PfCommon::rsaEncryption::rsaPubEncryptBase64(clearV, publicKey);
    qDebug() << encryptV;

    //利用私钥解密
    QString convertClearV = Pf::PfCommon::rsaEncryption::rsaPriDecryptBase64(encryptV, privateKey);
    qDebug() << convertClearV;

    return a.exec();
}
