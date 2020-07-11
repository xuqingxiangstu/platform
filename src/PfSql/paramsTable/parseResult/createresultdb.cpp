#include "createresultdb.h"

#include <QFile>

QMutex createResultDb::mInstanceMutex;
std::shared_ptr<createResultDb> createResultDb::mInstance = nullptr;

createResultDb::createResultDb()
{

}

createResultDb::~createResultDb()
{

}

QString createResultDb::create(const QString &uuid)
{
    QString dstFilePath = "./parseResult/" + uuid + ".db";

    if(QFile::exists(mTemplatePath) && !QFile::exists(dstFilePath))
    {
        QFile::copy(mTemplatePath, dstFilePath);
    }

    return dstFilePath;
}
