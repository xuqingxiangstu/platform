#include <QCoreApplication>

#include <xlsxdocument.h>
#include <xlsxworksheet.h>
#include <xlsxworkbook.h>

#include <sstream>
#include <QDebug>


/**
 * @brief excel读写例程
 * @example PfCommon/xlsxDemo/main.cpp
 */


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::ostringstream strErr;

#ifdef Utf8_Coding
    QXlsx::Document xlsx(QString::fromUtf8(path.c_str()));
#else
    QXlsx::Document xlsx("./frameCfg/Variable/VariableConfig.xlsx");
#endif
    QXlsx::Workbook *workBook = xlsx.workbook();

    QStringList sheetNames = xlsx.sheetNames();

    if(sheetNames.size() == 0)
    {
        qDebug() << "not found sheet";
    }

    for(auto name : sheetNames)
    {
        qDebug() << name;
    }

    return a.exec();
}
