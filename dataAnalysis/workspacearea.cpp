#include "workspacearea.h"
#include "ui_workspacearea.h"

workspaceArea::workspaceArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::workspaceArea)
{
    ui->setupUi(this);

    //mDataShowTable = new dataShowTable();
    //ui->verticalLayout_2->addWidget(mDataShowTable);
}

workspaceArea::~workspaceArea()
{
    delete ui;
}

void workspaceArea::loadDataShowWidget(QString uuid, QString proPath)
{
    //如果已有窗体则切换，否则新建窗体
    if(mDataShowWidgetManager.contains(uuid))
    {
        //已有则切
        ui->stackedWidget->setCurrentWidget(mDataShowWidgetManager[uuid]);
    }
    else
    {
        //新创建

        dataShowWidget *dataShow = new dataShowWidget();
        dataShow->setProUuidAndPath(uuid, proPath);

        mDataShowWidgetManager[uuid] = dataShow;

        ui->stackedWidget->addWidget(dataShow);
        ui->stackedWidget->setCurrentWidget(dataShow);
    }
}

void workspaceArea::onShowTableWidget(QString uuid, QString proPath)
{
    loadDataShowWidget(uuid, proPath);

    dynamic_cast<dataShowWidget*>(mDataShowWidgetManager[uuid])->onShowTableWidget();
}

void workspaceArea::onShowSingleImgWidget(QString uuid, QString proPath)
{
    loadDataShowWidget(uuid, proPath);

    dynamic_cast<dataShowWidget*>(mDataShowWidgetManager[uuid])->onShowSingleImgWidget();
}

void workspaceArea::onShowMultImgWidget(QString uuid, QString proPath)
{
    loadDataShowWidget(uuid, proPath);

    dynamic_cast<dataShowWidget*>(mDataShowWidgetManager[uuid])->onShowMultImgWidget();
}
