#include "manualtestdialog.h"
#include "ui_manualtestdialog.h"
#include <QMessageBox>

#include "icdAdapter/icdmanager.h"

#include "../src/PfSql/paramsTable/parseResult/resulttable.h"

#include <QDebug>
#include <QDateTime>

manualTestDialog::manualTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::manualTestDialog)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);

    mArgParse = std::make_shared<argumentParse>();
    connect(mArgParse.get(), &argumentParse::toShowResult, this, &manualTestDialog::onShowResult);
    connect(this, &manualTestDialog::toParse, mArgParse.get(), &argumentParse::parse);

    connect(mArgParse.get(), &argumentParse::showMessage, this, &manualTestDialog::onShowMessage);

    connect(ui->parseBtn, &QPushButton::clicked, this, [=](){
        QString text = ui->inputDataEdit->toPlainText();

        if(text.isEmpty())
        {
            QMessageBox::information(this, "提示", "请输入数据");
            return ;
        }

        //获取帧格式
        QString frameType = ui->comboBox->currentText();

        //解析

        QByteArray vaildMsg = QByteArray::fromHex(text.toLocal8Bit());
        int fType = icdManager::getInstance()->inverseMapping(frameType);
        if(fType == -1)
        {
            QString errorMsg = "获取(" + frameType + ")协议失败！";
            showMessage(errorMsg, false);
            QMessageBox::information(this, "提示", errorMsg);

            return ;
        }

        ui->parseBtn->setEnabled(false);

        clearResultTable();

        Json::Value param;
        param["frameType"] = fType;
        param["rowIndex"] = 0;

        emit toParse("", param, vaildMsg);
    });

    connect(ui->clearBtn, &QPushButton::clicked, this, [=](){
        ui->textEdit->clear();
    });

    ui->resultTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->resultTableWidget->horizontalHeader()->setStretchLastSection(true);  // 最后一列伸缩铺满

    connect(ui->clearInputBtn, &QPushButton::clicked, this, [=](){

        ui->inputDataEdit->clear();
    });
}

manualTestDialog::~manualTestDialog()
{
    delete ui;
}

void manualTestDialog::showMessage(QString msg, bool state)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    QString showMsg = "";

    //"<font color=\"#FF0000\">红色字体</font> "
    if(!state)
    {
        showMsg += "<font color=\"#FF0000\">";
        showMsg += str;
        showMsg += "-> ";
        showMsg += msg;
        showMsg += "</font>";
    }
    else
    {
        showMsg += str;
        showMsg += "-> ";
        showMsg += msg;
    }

    ui->textEdit->append(showMsg);

    if(!ui->parseBtn->isEnabled())
    {
        ui->parseBtn->setEnabled(true);
    }
}

void manualTestDialog::clearResultTable()
{
    //清空已有行
    for(int row = ui->resultTableWidget->rowCount() - 1;row >= 0; row--)
    {
        ui->resultTableWidget->removeRow(row);
    }
}

void manualTestDialog::onShowResult(QJsonObject value)
{
    //添加新行

    int rowcount = this->ui->resultTableWidget->rowCount();

    this->ui->resultTableWidget->insertRow(rowcount);//增加一行

    QTableWidgetItem *tmpItem = nullptr;

    int colNum = 0;

    tmpItem = new QTableWidgetItem(value.value(RESULT_TABLE_TABLE_NUM).toString());
    tmpItem->setFlags(Qt::NoItemFlags);
    this->ui->resultTableWidget->setItem(rowcount, colNum++, tmpItem);

    tmpItem = new QTableWidgetItem(QString::number(value.value(RESULT_TABLE_CODING_NUM).toInt(), 10));
    tmpItem->setFlags(Qt::NoItemFlags);
    this->ui->resultTableWidget->setItem(rowcount, colNum++, tmpItem);

    tmpItem = new QTableWidgetItem(value.value(RESULT_TABLE_NAME).toString());
    tmpItem->setFlags(Qt::NoItemFlags);
    this->ui->resultTableWidget->setItem(rowcount, colNum++, tmpItem);

    tmpItem = new QTableWidgetItem(value.value(RESULT_TABLE_HEX_VALUE).toString());
    tmpItem->setFlags(Qt::NoItemFlags);
    this->ui->resultTableWidget->setItem(rowcount, colNum++, tmpItem);

    tmpItem = new QTableWidgetItem(value.value(RESULT_TABLE_PARSE_VALUE).toString());
    tmpItem->setFlags(Qt::NoItemFlags);
    this->ui->resultTableWidget->setItem(rowcount, colNum++, tmpItem);

    tmpItem = new QTableWidgetItem(value.value(RESULT_TABLE_MEAN).toString());
    tmpItem->setFlags(Qt::NoItemFlags);
    this->ui->resultTableWidget->setItem(rowcount, colNum++, tmpItem);


    ui->parseBtn->setEnabled(true);
}

void manualTestDialog::onShowMessage(QString msg, bool state)
{
    showMessage(msg, state);
}
