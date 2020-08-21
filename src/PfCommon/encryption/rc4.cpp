#include "rc4.h"

#include <QFile>

namespace Pf
{
    namespace PfCommon
    {

        bool rc4::file_encrypt(const QString &file, const QString &key)
        {
            QByteArray decryptData;

            QFile fileObj(file);
            if(!fileObj.open(QIODevice::ReadWrite))
            {
                return false;
            }

            QByteArray clearData = fileObj.readAll();

            data_encrypt(clearData, decryptData, key);

            fileObj.resize(0);

            fileObj.write(decryptData);

            fileObj.close();

            return true;
        }

        void rc4::data_encrypt(const QByteArray &clearData, QByteArray &decryptData, const QString &key)
        {
            RC4_KEY s_table;
            RC4_set_key(&s_table, key.size(), (unsigned char  *)key.toUtf8().data());

            decryptData.resize(clearData.size());

            RC4(&s_table, clearData.size(), (const unsigned char  *)clearData.data(), (unsigned char*)decryptData.data());
        }

        void rc4::data_decrypt(const QByteArray &decryptData, QByteArray &clearData, const QString &key)
        {
            RC4_KEY s_table;
            clearData.resize(decryptData.size());

            RC4_set_key(&s_table, key.size(), (unsigned char  *)key.toUtf8().data());
            RC4(&s_table, decryptData.size(), (const unsigned char  *)decryptData.data(), (unsigned char  *)clearData.data());
        }

        bool rc4::file_decrypt(const QString &file, const QByteArray &key)
        {
            QByteArray clearData;

            QFile fileObj(file);
            if(!fileObj.open(QIODevice::ReadWrite))
            {
                return false;
            }

            QByteArray decryptData = fileObj.readAll();

            data_decrypt(decryptData, clearData, key);

            fileObj.resize(0);

            fileObj.write(clearData);

            fileObj.close();
        }
    }
}
