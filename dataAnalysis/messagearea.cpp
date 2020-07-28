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

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget,QTableWidget::customContextMenuRequested,[=](const QPoint &point){
        mPopMenu->exec(QCursor::pos());
    });

    mPopMenu = new QMenu(this);
    mPopMenu->addAction(ui->actionClear);

    connect(ui->actionClear, &QAction::triggered, this, [=](){
        //ui->tableWidget->clear();
        for(int row = ui->tableWidget->rowCount() - 1;row >= 0; row--)
        {
            ui->tableWidget->removeRow(row);
        }
    }, Qt::AutoConnection);
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

    this->ui->tableWidget->scrollToBottom();
}
