#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./fileAnalysis/analysis.h"
#include "./property/templateproperty.h"
#include "./argSave/saveDataBaseTask.h"
#include "manualtestdialog.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QEventLoop>
#include <QCloseEvent>
#include "newprjdialog.h"

#include <QEventLoop>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isExit(false)
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
    if(recentlyPrj.size() > 0)
    {
        ui->useredMenu->addSeparator();
        ui->useredMenu->addAction(ui->actionClearMenu);
        ui->useredMenu->setEnabled(true);
    }
    else
    {
        ui->useredMenu->setEnabled(false);
    }

    connect(ui->actionClearMenu, &QAction::triggered, [=](){
       ui->useredMenu->clear();
       QSettings prjSettings("BJTU", "dataAnalysis");
       QStringList nullList = {};
       prjSettings.setValue("recently_used", nullList);
       ui->useredMenu->setEnabled(false);
    });

    connect(ui->useredMenu, &QMenu::triggered, [=](QAction *action){
        if(action == ui->actionClearMenu)
            return ;

        QString prjPath = action->text();
        loadProject(prjPath);
    });

    connect(ui->actionExit, &QAction::triggered, [=](){
       //exit(1);
        close();
    });

    connect(ui->actionOpenPrject, &QAction::triggered, [=](){
        QFileDialog fileDialog(this);
        //设置窗口的标题
        fileDialog.setWindowTitle("工程文件选择");
        fileDialog.setNameFilter("工程文件(*.prj)"); //设置一个过滤器

        //这个标志用来设置选择的类型，比如默认是单个文件。QFileDialog::ExistingFiles 多个文件,还可以用来选择文件夹QFileDialog::Directory。QFileDialog::ExistingFile 单个文件。注意这个ExistingFile，单词后面多了一个s 表示选择多个文件。要看清楚了。
        fileDialog.setFileMode(QFileDialog::ExistingFile);

        //弹出对话框
        if (fileDialog.exec() == QDialog::Accepted)
        {
            QStringList srcFiles = fileDialog.selectedFiles();
            loadProject(srcFiles[0]);
        }
    });

    //初始化工作区
    mWorkSpaceAreaWidget = new workspaceArea(this);
    ui->workSpaceVerticalLayout->addWidget(mWorkSpaceAreaWidget);

    //初始化导航区
    mProjectNavigationWidget = new projectNavigation(this);
    ui->navigationVerticalLayout->addWidget(mProjectNavigationWidget);

    connect(this, &MainWindow::saveProject, mProjectNavigationWidget, &projectNavigation::onSaveProject);

    connect(mProjectNavigationWidget, &projectNavigation::closeProject, mWorkSpaceAreaWidget, &workspaceArea::onCloseProject);
    connect(mProjectNavigationWidget, &projectNavigation::showMultImgWidget, mWorkSpaceAreaWidget, &workspaceArea::onShowMultImgWidget);
    connect(mProjectNavigationWidget, &projectNavigation::showSingleImgWidget, mWorkSpaceAreaWidget, &workspaceArea::onShowSingleImgWidget);
    connect(mProjectNavigationWidget, &projectNavigation::showTableWidget, mWorkSpaceAreaWidget, &workspaceArea::onShowTableWidget);
    connect(mProjectNavigationWidget, &projectNavigation::showFileWidget, mWorkSpaceAreaWidget, &workspaceArea::onShowFileWidget);

    //初始化消息区
    mMessageAreaWidget = new messageArea(this);
    ui->messageVerticalLayout->addWidget(mMessageAreaWidget);

    connect(mProjectNavigationWidget, &projectNavigation::showMessage, mMessageAreaWidget, &messageArea::showMessage);
    connect(mWorkSpaceAreaWidget, &workspaceArea::showMessage, mMessageAreaWidget, &messageArea::showMessage);

    connect(mProjectNavigationWidget, &projectNavigation::saveProjectOver, this, &MainWindow::onProjectAlreadySave);

    //初始化属性区
    mNavigationPropertyObj = new propertyWidget();
    ui->propertyVerticalLayout->addWidget(mNavigationPropertyObj);

    connect(mProjectNavigationWidget, &projectNavigation::toShowProperty, mNavigationPropertyObj, &propertyWidget::showProperty);
    connect(mNavigationPropertyObj, &propertyWidget::valueChange, mProjectNavigationWidget, &projectNavigation::onPropertyValueChange);

    try
    {
        templateProperty::getInstance()->init("./cfgfile/analysis-property.json");
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

    connect(ui->actionSave, &QAction::triggered, [=](){
        emit saveProject("");
    });

    mVersionWidget = new versionForm();

    ui->mainToolBar->addWidget(mVersionWidget);

    connect(ui->actionManualTest, &QAction::triggered, [=]{
        manualTestDialog mTDlg;
        mTDlg.exec();
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::No == QMessageBox::warning(this, "提示", "是否退出",QMessageBox::Yes | QMessageBox::No))
    {
        event->ignore();
        return ;
    }

    if(mProjectNavigationWidget)
    {
        mSaveProjectUuid.clear();
        mSaveProjectUuid = mProjectNavigationWidget->getModifyProject();
        if(mSaveProjectUuid.size() > 0)
        {
            if(QMessageBox::Yes == QMessageBox::warning(this, "提示", "有未保存的更改，是否保存",QMessageBox::Yes | QMessageBox::No))
            {
                isExit = true;

                foreach (QString uuid, mSaveProjectUuid)
                {
                    //保存
                    emit saveProject(uuid);
                }

                event->ignore();
                return ;
            }
        }
    }
    event->accept();
}

void MainWindow::onProjectAlreadySave(QString uuid)
{
    if(isExit)
    {
        mSaveProjectUuid.removeOne(uuid);

        if(mSaveProjectUuid.size() == 0)
            exit(1);
    }
}

void MainWindow::loadProject(const QString &path)
{
    QSettings prjSettings("BJTU", "dataAnalysis");
    QStringList recentlyPrj = prjSettings.value("recently_used").toStringList();

    //判断是否存在
    if(!QFile::exists(path))
    {
        //删除记录

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
        if(!recentlyPrj.contains(path))
            recentlyPrj.insert(0, path);

        prjSettings.setValue("recently_used", recentlyPrj);

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
