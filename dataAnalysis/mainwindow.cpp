#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./fileAnalysis/analysis.h"
#include "./property/templateproperty.h"
#include "./argSave/saveDataBaseTask.h"

#include <QEventLoop>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    

    setWindowState(Qt::WindowMaximized);

    //删除中央窗体
    QWidget* centralW = takeCentralWidget();
    if(centralW)
    {
        delete centralW;
        centralW = nullptr;
    }

    //初始化工作区
    mWorkSpaceAreaWidget = new workspaceArea(this);
    ui->workSpaceVerticalLayout->addWidget(mWorkSpaceAreaWidget);

    //初始化导航区
    mProjectNavigationWidget = new projectNavigation(this);
    ui->navigationVerticalLayout->addWidget(mProjectNavigationWidget);


    //初始化消息区
    mMessageAreaWidget = new messageArea(this);
    ui->messageVerticalLayout->addWidget(mMessageAreaWidget);

    //初始化属性区
    mNavigationPropertyObj = new propertyWidget();
    ui->propertyVerticalLayout->addWidget(mNavigationPropertyObj);

    connect(mProjectNavigationWidget, &projectNavigation::toShowProperty, mNavigationPropertyObj, &propertyWidget::showProperty);

    try
    {
        templateProperty::getInstance()->init("./cfgfile/analysis-property.json");

        Json::Value logJs = templateProperty::getInstance()->getProperty("log");

        mNavigationPropertyObj->showProperty("", logJs);

    }
    catch(std::runtime_error err)
    {
        //qDebug() << err.what();
        QMessageBox::information(this, "提示", QString::fromStdString(err.what()));
        exit(0);
    }

   // test();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{
    QEventLoop lp;

    analysis analy;
    std::shared_ptr<analysisRule> rule = std::make_shared<analysisRule>();
    rule->setSegmentationMark("\n");

    //filterManager *fm = new

    std::shared_ptr<saveDataBaseTask > saveTask = std::make_shared<saveDataBaseTask>();

    connect(&analy, &analysis::toDataBase, saveTask.get(), &saveDataBaseTask::onWrite);

    connect(saveTask.get(), &saveDataBaseTask::noMsg, &lp, &QEventLoop::quit);

    saveTask->startTask("test");

    analy.onAnalysis("1", "./logRecord/前端To信息处理1/2020-06-30/09.37.01.692.dat", rule, nullptr);

    saveTask->onOver();

    lp.exec();
}
