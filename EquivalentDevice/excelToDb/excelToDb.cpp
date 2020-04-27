#include "excelToDb.h"

#include <xlsxdocument.h>
#include <xlsxworksheet.h>
#include <xlsxworkbook.h>
#include <sstream>
#include <QDebug>

#include <QSqlQuery>
#include <QVariant>
#include <QFileInfo>
#include <QMessageBox>
#include "../dbtableopt.h"

excelToDb::excelToDb(QObject *parent)
	: QThread(parent)
{

}

excelToDb::~excelToDb()
{

}

void excelToDb::run()
{
    bool result = false;
    std::vector<QString> msgBox;
    QXlsx::Document xlsx(strFilePath);
    QXlsx::Workbook *workBook = xlsx.workbook();
    int sheetCount = workBook->sheetCount();
    DBTableOpt::getInstance()->deleteTable("params_table");
    QString value;
    QString strErr;
    if(sheetCount == 1){
       emit setRange(0, 1);
    }else{
       emit setRange(0, sheetCount-1);
    }

    for(int h =0;h<sheetCount;h++)
    {
        QXlsx::Worksheet *workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(h));
        QString tableName = workSheet->sheetName();
        for (int i = 3; i <=workSheet->dimension().lastRow(); i++)
        {
            msgBox.clear();
            msgBox.push_back(tableName);
            QXlsx::Cell *cellbreak = workSheet->cellAt(i, 1);
            if(cellbreak == NULL){
                continue;
            }else{
                for (int j = 1; j <= 18; j++)
                {
                    QXlsx::Cell *cell = workSheet->cellAt(i, j);
                    if (cell==NULL)
                    {
                        value = "";
                    }else{
                        value = cell->value().toString();
                    }
                    msgBox.push_back(value);
                }
                result &= DBTableOpt::getInstance()->excelToDb(msgBox);
            }
        }
        if(sheetCount == 1){
            emit setCurValue(1);
        }else{
            emit setCurValue(h);
        }

    }
END:
		
	emit setEnd(strErr);

	return;
}
