#include "datashowwidget.h"
#include "ui_datashowwidget.h"

#include "datashowtable.h"
#include "dssingleimg.h"
#include "dsmuiltimg.h"

dataShowWidget::dataShowWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dataShowWidget)
{
    ui->setupUi(this);
}

dataShowWidget::~dataShowWidget()
{
    delete ui;
}


void dataShowWidget::onShowTableWidget()
{
    if(mTabWidgetManager.contains(TABLE_WIDGET))
    {
        ui->tabWidget->setCurrentWidget(mTabWidgetManager[TABLE_WIDGET]);
    }
    else
    {
        dataShowTable *tableW = new dataShowTable(mProUuid, mProPath);

        mTabWidgetManager[TABLE_WIDGET] = tableW;
        ui->tabWidget->addTab(tableW, TABLE_WIDGET);
        ui->tabWidget->setCurrentWidget(tableW);
    }
}

void dataShowWidget::onShowSingleImgWidget()
{
    if(mTabWidgetManager.contains(S_IMG_WIDGET))
    {
        ui->tabWidget->setCurrentWidget(mTabWidgetManager[S_IMG_WIDGET]);
    }
    else
    {
        dsSingleImg *tableW = new dsSingleImg(mProUuid, mProPath);

        mTabWidgetManager[S_IMG_WIDGET] = tableW;
        ui->tabWidget->addTab(tableW, S_IMG_WIDGET);
        ui->tabWidget->setCurrentWidget(tableW);
    }
}

void dataShowWidget::onShowMultImgWidget()
{
    if(mTabWidgetManager.contains(M_IMG_WIDGET))
    {
        ui->tabWidget->setCurrentWidget(mTabWidgetManager[M_IMG_WIDGET]);
    }
    else
    {
        dsMuiltImg *tableW = new dsMuiltImg(mProUuid, mProPath);

        mTabWidgetManager[M_IMG_WIDGET] = tableW;
        ui->tabWidget->addTab(tableW, M_IMG_WIDGET);
        ui->tabWidget->setCurrentWidget(tableW);
    }
}
