#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../src/PfSql/paramsTable/systemtable.h"
#include "./property/templateproperty.h"
#include "../src/PfSql/paramsTable/flowrecordtable.h"

#include <QDebug>
#include <QDir>

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
    connect(this, &MainWindow::saveProject, mRecordNavigationObj, &recordNavigation::onSaveProject);

    mNavigationPropertyObj = new propertyWidget();
    ui->navigationProptertyVerticalLayout->addWidget(mNavigationPropertyObj);
    connect(mRecordNavigationObj, &recordNavigation::toShowProperty, mNavigationPropertyObj, &propertyWidget::showProperty);
    connect(mNavigationPropertyObj, &propertyWidget::valueChange, mRecordNavigationObj, &recordNavigation::onPropertyValueChange);

    mRecordNavigationObj->buildTree();
    ui->navigationVerticalLayout->addWidget(mRecordNavigationObj);

    mPropertyWidgetObj = new propertyWidget();
    ui->propertyWidgetHorizontalLayout->addWidget(mPropertyWidgetObj);

    connect(mPropertyWidgetObj, &propertyWidget::valueChange, this, &MainWindow::valueChange);
    connect(this, &MainWindow::toShowProperty, mPropertyWidgetObj, &propertyWidget::showProperty);
    connect(this, &MainWindow::updateProperty, mPropertyWidgetObj, &propertyWidget::onUpdateProperty);

    connect(ui->actionSave, &QAction::triggered, [=](){
        onSave();
    });
}


void MainWindow::onDeleteFlow(QString uuid)
{   
    if(mFlowWidgetManager.contains(uuid))
    {
        //删除窗体
        QWidget *item = mFlowWidgetManager[uuid];
        if(item)
        {
            mFlowWidgetManager.remove(uuid);

            ui->stackedWidget->removeWidget(item);
        }

        //删除文件
        QString filePath = "./flowFile/" + uuid + ".xml";
        QDir dir;
        dir.remove(filePath);
    }
}

void MainWindow::initNavigationProperty()
{

}

void MainWindow::onSave()
{
    //发送保存信号
    if(mRecordNavigationObj->isModify())
        emit saveProject(mRecordNavigationObj->curItem());
}

void MainWindow::onFlowChange(QString sysName, int sysType, QString testName, QString uuid, bool isNew)
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
        flowTree *flowWidget = new flowTree(uuid, sysType);

        flowWidget->setSystemAndProductName(sysName, testName);

        connect(this, &MainWindow::valueChange, flowWidget, &flowTree::onPropertyValueChange);

        connect(this, &MainWindow::saveProject, flowWidget, &flowTree::onSaveProject);
        connect(flowWidget, &flowTree::toShowProperty, this, &MainWindow::toShowProperty);
        connect(flowWidget, &flowTree::projectModify, mRecordNavigationObj, &recordNavigation::onProjectModify);
        connect(flowWidget, &flowTree::saveProjectOver, mRecordNavigationObj, &recordNavigation::onProjectAlreadySave);
        connect(this, &MainWindow::updateProject, flowWidget, &flowTree::onUpdateProject);
        connect(flowWidget, &flowTree::updateProperty, this, &MainWindow::updateProperty);

        mFlowWidgetManager[uuid] = flowWidget;

        ui->stackedWidget->addWidget(flowWidget);
        ui->stackedWidget->setCurrentWidget(flowWidget);
    }    

    //更新树(新创建的则不用更新)
    if(!isNew)
        emit updateProject(uuid);

    mCurFlowWidgetUuid = uuid;
}

MainWindow::~MainWindow()
{
    delete ui;
}
