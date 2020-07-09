#include "createresultdb.h"

#include <QFile>

QMutex createResultDb::mInstanceMutex;
std::shared_ptr<createResultDb> createResultDb::mInstance = nullptr;

createResultDb::createResultDb()
{

}

void createResultDb::create(const QString &uuid)
{
    QString dstFilePath = "./parseResult/" + uuid + ".db";

    if(QFile::exists(mTemplatePath))
    {
        QFile::copy(mTemplatePath, dstFilePath);
    }
}
