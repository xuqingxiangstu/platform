#include "copyxml.h"
#include <QFile>

void copyXml::copy(const QString &srcFilePath, const QString &dstFilePath)
{
    if(QFile::exists(dstFilePath))
    {
        QFile::remove(dstFilePath);
    }

    if(QFile::exists(srcFilePath))
    {
        QFile::copy(srcFilePath, dstFilePath);
    }
}
