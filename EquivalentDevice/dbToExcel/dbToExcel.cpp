#include "dbToExcel.h"

#include <xlsxdocument.h>
#include <xlsxworksheet.h>
#include <xlsxworkbook.h>
#include "xlsxabstractsheet.h"
#include <sstream>
#include <QDebug>

#include <QSqlQuery>
#include <QVariant>
#include <QFileInfo>
#include <QMessageBox>
#include "../dbtableopt.h"

#include <xlsxcellformula.h>
#include <xlsxdatavalidation.h>
using namespace QXlsx;
dbToExcel::dbToExcel(QObject *parent)
	: QThread(parent)
{

}

dbToExcel::~dbToExcel()
{

}

void dbToExcel::run()
{
    bool result = false;
    QString strErr;
    std::vector<QString> msgBox;
    msgBox.clear();
    DBTableOpt::getInstance()->dbToExcelTableNum(msgBox);

    if(msgBox.size() == 1){
        emit setRange(0, 1);
    }else{
        emit setRange(0, msgBox.size()-1);
    }


    QXlsx::Document xlsx(strFilePath);
    QXlsx::Workbook *workBook = xlsx.workbook();
    QXlsx::Worksheet *workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(0));
    QString firstName =workSheet->sheetName();
    for(int i = 0;i<msgBox.size();i++){
        xlsx.copySheet(firstName,msgBox[i]);
    }

    DataValidation FType(DataValidation::List,DataValidation::Equal,"\"FE,BE,93\"");
    FType.addRange("B2:B10000");
    FType.setPromptMessage("Please Input choose between BE, FE, 93");

    DataValidation InfoWord(DataValidation::List,DataValidation::Equal,"\"0,1,2\"");
    InfoWord.addRange("C2:C10000");
    InfoWord.setPromptMessage("Please Input choose between 0, 1, 2");

    DataValidation ISACK(DataValidation::List,DataValidation::Equal,"\"是,否\"");
    ISACK.addRange("D2:D10000");
    ISACK.setPromptMessage("Please Input choose between 是,否");

    DataValidation CMDTYPE(DataValidation::List,DataValidation::Equal,"\"指令,状态量,数值量\"");
    CMDTYPE.addRange("E2:E10000");
    CMDTYPE.setPromptMessage("Please Input choose between 指令,状态量,数值量");

    DataValidation LBENDIAN(DataValidation::List,DataValidation::Equal,"\"大端,小端\"");
    LBENDIAN.addRange("L2:L10000");
    LBENDIAN.setPromptMessage("Please Input choose between 大端,小端");

    DataValidation DATATYPE(DataValidation::List,DataValidation::Equal,"\"UINT16,UINT8,UINT32,IEEE32,IEEE64,NCHAR,NRAW\"");
    DATATYPE.addRange("P2:P10000");
    DATATYPE.setPromptMessage("Please Input choose between UINT16,UINT8,UINT32,IEEE32,IEEE64,NCHAR");

    for(int i = 1;i<=msgBox.size();i++){
        QXlsx::Worksheet *writeSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(i));
        QString sheetName = writeSheet->sheetName();
        Json::Value paramsInfo;
        DBTableOpt::getInstance()->dbToExcel(sheetName,paramsInfo);
        if(sheetName == "50019"){
            paramsInfo.toStyledString();
            std::string out = Json::FastWriter().write(paramsInfo);
            qDebug()<<QString::fromStdString(out);
        }

        for(int j = 0;j<paramsInfo.size();j++){
            writeSheet->write(j+3,CODING_NUM,QString::fromStdString(paramsInfo[j]["CODING_NUM"].asString()));
            writeSheet->write(j+3,FRAME_TYPE,QString::fromStdString(paramsInfo[j]["FRAME_TYPE"].asString()));
            writeSheet->write(j+3,INFO_WORD_TYPE,QString::fromStdString(paramsInfo[j]["INFO_WORD_TYPE"].asString()));
            writeSheet->write(j+3,IS_ACK,QString::fromStdString(paramsInfo[j]["IS_ACK"].asString()));
            writeSheet->write(j+3,CMD_TYPE,QString::fromStdString(paramsInfo[j]["CMD_TYPE"].asString()));
            writeSheet->write(j+3,GROUP_NAME,QString::fromStdString(paramsInfo[j]["GROUP_NAME"].asString()));
            writeSheet->write(j+3,PARAM_NAME,QString::fromStdString(paramsInfo[j]["PARAM_NAME"].asString()));
            writeSheet->write(j+3,PARAM_BYTE_START_POS,QString::fromStdString(paramsInfo[j]["PARAM_BYTE_START_POS"].asString()));
            writeSheet->write(j+3,PARAM_BYTE_SIZE,QString::fromStdString(paramsInfo[j]["PARAM_BYTE_SIZE"].asString()));
            writeSheet->write(j+3,PARAM_BIT_START_POS,QString::fromStdString(paramsInfo[j]["PARAM_BIT_START_POS"].asString()));
            writeSheet->write(j+3,PARAM_BIT_SIZE,QString::fromStdString(paramsInfo[j]["PARAM_BIT_SIZE"].asString()));
            writeSheet->write(j+3,L_B_ENDIAN,QString::fromStdString(paramsInfo[j]["L_B_ENDIAN"].asString()));
            writeSheet->write(j+3,INIT_VALUE,QString::fromStdString(paramsInfo[j]["INIT_VALUE"].asString()));
            writeSheet->write(j+3,MIN_VALUE,QString::fromStdString(paramsInfo[j]["MIN_VALUE"].asString()));
            writeSheet->write(j+3,MAX_VALUE,QString::fromStdString(paramsInfo[j]["MAX_VALUE"].asString()));
            writeSheet->write(j+3,DATA_TYPE,QString::fromStdString(paramsInfo[j]["DATA_TYPE"].asString()));
            writeSheet->write(j+3,SYSTEM,QString::fromStdString(paramsInfo[j]["SYSTEM"].asString()));
            writeSheet->write(j+3,VALUE_MEAN,QString::fromStdString(paramsInfo[j]["VALUE_MEAN"].asString()));

            writeSheet->addDataValidation(FType);
            writeSheet->addDataValidation(InfoWord);
            writeSheet->addDataValidation(ISACK);
            writeSheet->addDataValidation(CMDTYPE);
            writeSheet->addDataValidation(LBENDIAN);
            writeSheet->addDataValidation(DATATYPE);
        }
        if(msgBox.size() == 1){
            emit setCurValue(1);
        }else{
            emit setCurValue(i);
        }
    }
    workBook->deleteSheet(0);
    xlsx.saveAs(strFilePath);


END:
		
	emit setEnd(strErr);

	return;
}
