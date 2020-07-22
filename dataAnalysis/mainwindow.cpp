#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./fileAnalysis/analysis.h"
#include "./property/templateproperty.h"
#include "./argSave/saveDataBaseTask.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QEventLoop>
#include "newprjdialog.h"

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
//    QWidget* centralW = takeCentralWidget();
//    if(centralW)
//    {
//        delete centralW;
//        centralW = nullptr;
//    }

    setCentralWidget(0);

    //this->resizeDocks({ui->dockWidget, ui->dockWidget_2},{200, 100},Qt::Horizontal);

    //加载最近使用项目
    //ui->actionUsedPrj
    QSettings prjSettings("BJTU", "dataAnalysis");
    QStringList recentlyPrj = prjSettings.value("recently_used").toStringList();
    for(QString path : recentlyPrj)
    {
        QAction *tmpAction = new QAction(this);
        tmpAction->setText(path);
        ui->useredMenu->addAction(tmpAction);
    }

    connect(ui->useredMenu, &QMenu::triggered, [=](QAction *action){
        QString prjPath = action->text();
        loadProject(prjPath);
    });

    //初始化工作区
    mWorkSpaceAreaWidget = new workspaceArea(this);
    ui->workSpaceVerticalLayout->addWidget(mWorkSpaceAreaWidget);

    //初始化导航区
    mProjectNavigationWidget = new projectNavigation(this);
    ui->navigationVerticalLayout->addWidget(mProjectNavigationWidget);

    connect(mProjectNavigationWidget, &projectNavigation::showMultImgWidget, mWorkSpaceAreaWidget, &workspaceArea::onShowMultImgWidget);
    connect(mProjectNavigationWidget, &projectNavigation::showSingleImgWidget, mWorkSpaceAreaWidget, &workspaceArea::onShowSingleImgWidget);
    connect(mProjectNavigationWidget, &projectNavigation::showTableWidget, mWorkSpaceAreaWidget, &workspaceArea::onShowTableWidget);

    //初始化消息区
    mMessageAreaWidget = new messageArea(this);
    ui->messageVerticalLayout->addWidget(mMessageAreaWidget);

    connect(mProjectNavigationWidget, &projectNavigation::showMessage, mMessageAreaWidget, &messageArea::showMessage);
    connect(mWorkSpaceAreaWidget, &workspaceArea::showMessage, mMessageAreaWidget, &messageArea::showMessage);

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

    //test();

    connect(ui->actionNewPrj, &QAction::triggered, [=](){
        newPrjDialog dlg;
        int res = dlg.exec();
        qDebug() << res;
        if(QDialog::Accepted == res)
        {
            QString name = dlg.getPrjName();
            QString path = dlg.getPrjPath();
            mProjectNavigationWidget->createProject(name, path);
        }
        else
        {

        }
    });


#if 1
    //TODO:测试
   // projectCfg
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadProject(const QString &path)
{
    //判断是否存在
    if(!QFile::exists(path))
    {
        //删除记录

        QSettings prjSettings("BJTU", "dataAnalysis");
        QStringList recentlyPrj = prjSettings.value("recently_used").toStringList();
        //recentlyPrj.append(proPath + "/" + name + "/" + name + ".prj");

        if(recentlyPrj.contains(path))
        {
            recentlyPrj.removeOne(path);
        }

        prjSettings.setValue("recently_used", recentlyPrj);

        //提示用户
        QString info;
        info += "Failed opening project \"";
        info += path;
        info += "\":project is not exists";
        QMessageBox::information(this, "提示", info);
    }
    else
    {
        mProjectNavigationWidget->loadProject(path);
    }
}

void MainWindow::test()
{
#if 0
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
#endif
}
