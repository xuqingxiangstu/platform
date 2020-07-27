#include "projecttemplate.h"

#include <QDir>
#include <QFile>

#include "nullxml.h"

void projectTemplate::create(const QString &name, const QString &path, const QString &dbName)
{
    //step1：创建工程文件夹
    QDir tmp;
    QString dstPath = path;

    tmp.mkdir(dstPath);

    //step2：创建data文件夹

    QString dataPath = dstPath + "/data";
    tmp.mkdir(dataPath);

    //setp3：创建result文件夹

    QString resultPath = dstPath + "/result";
    tmp.mkdir(resultPath);

    //step4：在result文件夹中创建数据库文件，从已有模板中拷贝一份

    QString dbPath = resultPath + "/" + dbName + ".db";
    QString mTemplatePath = "./cfgfile/dataAnalysis.db";

    if(QFile::exists(mTemplatePath) && !QFile::exists(dbPath))
    {
        QFile::copy(mTemplatePath, dbPath);
    }

    //setp5：创建.prj工程文件

    QString prjFile = dstPath + "/" + name + ".prj";

    nullXml::create(prjFile);
}
