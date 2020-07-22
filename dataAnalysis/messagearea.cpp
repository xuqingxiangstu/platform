#include "messagearea.h"
#include "ui_messagearea.h"
#include <QDateTime>

messageArea::messageArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::messageArea)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 200);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

messageArea::~messageArea()
{
    delete ui;
}


void messageArea::showMessage(QString msg, bool state)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");

    if(state)
    {
        int rowcount = this->ui->tableWidget->rowCount();

        this->ui->tableWidget->insertRow(rowcount);//增加一行
        QTableWidgetItem *strItem = new QTableWidgetItem(str);
        strItem->setFlags(Qt::NoItemFlags);

        this->ui->tableWidget->setItem(rowcount,0,strItem);

        QTableWidgetItem *msgItem = new QTableWidgetItem(msg);
        msgItem->setFlags(Qt::NoItemFlags);

        this->ui->tableWidget->setItem(rowcount,1,msgItem);
    }
    else
    {
        int rowcount = this->ui->tableWidget->rowCount();
        this->ui->tableWidget->insertRow(rowcount);//增加一行

        QTableWidgetItem *strItem = new QTableWidgetItem(str);
        strItem->setFlags(Qt::NoItemFlags);

        this->ui->tableWidget->setItem(rowcount,0,strItem);

        QTableWidgetItem *msgItem = new QTableWidgetItem(msg);
        msgItem->setFlags(Qt::NoItemFlags);

        this->ui->tableWidget->setItem(rowcount,1,msgItem);
        this->ui->tableWidget->item(rowcount,0)->setTextColor(QColor(255,0,0));
        this->ui->tableWidget->item(rowcount,1)->setTextColor(QColor(255,0,0));
    }
}
