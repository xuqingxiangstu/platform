#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../src/PfSql/paramsTable/systemtable.h"
#include "./property/templateproperty.h"
#include "../src/PfSql/paramsTable/flowrecordtable.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);

    try
    {
        templateProperty::getInstance()->init("./cfgfile/property.json");
    }
    catch(std::runtime_error err)
    {
        qDebug() << err.what();
    }

    mTemplateTreeObj = new templateTree();
    ui->templateTreeHorizontalLayout->addWidget(mTemplateTreeObj);

    mRecordNavigationObj = new recordNavigation();
    connect(mRecordNavigationObj, &recordNavigation::flowChange, this, &MainWindow::onFlowChange);
    connect(mRecordNavigationObj, &recordNavigation::deleteFlow, this, &MainWindow::onDeleteFlow);

    mRecordNavigationObj->buildTree();
    ui->navigationVerticalLayout->addWidget(mRecordNavigationObj);

    mPropertyWidgetObj = new propertyWidget();
    ui->propertyWidgetHorizontalLayout->addWidget(mPropertyWidgetObj);

    connect(mPropertyWidgetObj, &propertyWidget::valueChange, this, &MainWindow::valueChange);
    connect(this, &MainWindow::toShowProperty, mPropertyWidgetObj, &propertyWidget::showProperty);

    connect(ui->actionSave, &QAction::triggered, [=](){
        onSave();
    });
}

void MainWindow::onDeleteFlow(QString uuid)
{
    //TODO:如果已修改则提示用户是否保存

    if(mFlowWidgetManager.contains(uuid))
    {
        QWidget *item = mFlowWidgetManager[uuid];
        if(item)
        {
            mFlowWidgetManager.remove(uuid);

            ui->stackedWidget->removeWidget(item);
        }
    }
}

void MainWindow::onSave()
{

}

void MainWindow::onFlowChange(QString sysName, QString testName, QString uuid)
{
    //如果已有窗体则切换，否则新建窗体
    if(mFlowWidgetManager.contains(uuid))
    {
        //已有则切
        ui->stackedWidget->setCurrentWidget(mFlowWidgetManager[uuid]);
    }
    else
    {
        //新创建
        flowTree *flowWidget = new flowTree(uuid);

        flowWidget->setSystemAndProductName(sysName, testName);

        connect(this, &MainWindow::valueChange, flowWidget, &flowTree::onPropertyValueChange);
        connect(flowWidget, &flowTree::toShowProperty, this, &MainWindow::toShowProperty);

        mFlowWidgetManager[uuid] = flowWidget;

        ui->stackedWidget->addWidget(flowWidget);
        ui->stackedWidget->setCurrentWidget(flowWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == (Qt::ControlModifier) && e->key() == Qt::Key_S)
    {
        qDebug() << "Ctrl + S";
    }
}
