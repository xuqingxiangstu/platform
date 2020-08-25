#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../src/PfSql/paramsTable/systemtable.h"
#include "./property/templateproperty.h"
#include "../src/PfSql/paramsTable/flowrecordtable.h"

#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isExit(false)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);

    mCmdDecodeObj = std::make_shared<cmdDecode>();
    mCmdDecodeObj->startDecode();

    try
    {
        templateProperty::getInstance()->init("./cfgfile/property.json");


    mPropertyWidgetObj = new propertyWidget();
    ui->propertyWidgetHorizontalLayout->addWidget(mPropertyWidgetObj);

    mTemplateTreeObj = new templateTree();
    ui->templateTreeHorizontalLayout->addWidget(mTemplateTreeObj);

    mRecordNavigationObj = new recordNavigation();
    connect(mRecordNavigationObj, &recordNavigation::flowChange, this, &MainWindow::onFlowChange);
    connect(mRecordNavigationObj, &recordNavigation::deleteFlow, this, &MainWindow::onDeleteFlow);
    connect(this, &MainWindow::saveProject, mRecordNavigationObj, &recordNavigation::onSaveProject);

    mNavigationPropertyObj = new propertyWidget();
    ui->navigationProptertyVerticalLayout->addWidget(mNavigationPropertyObj);

    connect(mRecordNavigationObj, &recordNavigation::toShowProperty, mNavigationPropertyObj, &propertyWidget::showProperty);

    connect(mRecordNavigationObj, &recordNavigation::setGroupPropertyEnable, mNavigationPropertyObj, &propertyWidget::setGroupPropertyEnable);

    connect(mRecordNavigationObj, &recordNavigation::removeProperty, mNavigationPropertyObj, &propertyWidget::removeProperty);
    connect(mRecordNavigationObj, &recordNavigation::removeGroupProperty, mNavigationPropertyObj, &propertyWidget::removeGroupProperty);
    connect(mRecordNavigationObj, &recordNavigation::addProperty, mNavigationPropertyObj, &propertyWidget::addProperty);
    connect(mRecordNavigationObj, &recordNavigation::addGroupProperty, mNavigationPropertyObj, &propertyWidget::addGroupProperty);

    connect(mNavigationPropertyObj, &propertyWidget::valueChange, mRecordNavigationObj, &recordNavigation::onPropertyValueChange);


    mRecordNavigationObj->buildTree();
    ui->navigationVerticalLayout->addWidget(mRecordNavigationObj);


    connect(mRecordNavigationObj, &recordNavigation::clearFlowProperty, mPropertyWidgetObj, &propertyWidget::showProperty);    

    connect(mPropertyWidgetObj, &propertyWidget::valueChange, this, &MainWindow::valueChange);
    connect(this, &MainWindow::toShowProperty, mPropertyWidgetObj, &propertyWidget::showProperty);
    connect(this, &MainWindow::updateProperty, mPropertyWidgetObj, &propertyWidget::onUpdateProperty);

    connect(ui->actionSave, &QAction::triggered, [=](){
        onSave();
    });
        mVersionWidget = new versionForm();

        ui->mainToolBar->addWidget(mVersionWidget);
    }
    catch(std::runtime_error err)
    {
        //qDebug() << err.what();
        QMessageBox::information(this, "提示", QString::fromStdString(err.what()));
        exit(0);
    }
    catch(Json::LogicError err)
    {
        QMessageBox::information(this, "提示", QString::fromStdString(err.what()));
        exit(0);
    }
    catch(...)
    {
        QMessageBox::information(this, "提示", "[ERROR]...");
        exit(0);
    }

    connect(ui->actionDefaultLayout, &QAction::triggered, this, [this](){

        if(ui->pjtDock->isFloating())
            ui->pjtDock->setFloating(false);

        if(ui->pjtAttrDock->isFloating())
            ui->pjtAttrDock->setFloating(false);

        if(ui->templateDock->isFloating())
            ui->templateDock->setFloating(false);

        if(ui->workAttrDock->isFloating())
            ui->workAttrDock->setFloating(false);

        addDockWidget(Qt::LeftDockWidgetArea,ui->pjtDock);
        addDockWidget(Qt::LeftDockWidgetArea,ui->pjtAttrDock);
        addDockWidget(Qt::RightDockWidgetArea,ui->templateDock);
        addDockWidget(Qt::RightDockWidgetArea,ui->workAttrDock);
        //splitDockWidget();
    });

    mSearchForm = new searchForm();
    ui->horizontalLayout->addWidget(mSearchForm);

    connect(this, &MainWindow::searchResult, mSearchForm, &searchForm::onSearchResult);
    connect(mSearchForm, &searchForm::toSearch, this, &MainWindow::onSearch);
    connect(mSearchForm, &searchForm::positionResult, this, &MainWindow::onPositionResult);
    connect(mSearchForm, &searchForm::closeSearch, this, [=](){

        ui->frame->hide();
    });

    connect(ui->actionSearch, &QAction::triggered, this, [=](){
       if(ui->frame->isHidden())
           ui->frame->show();

       mSearchForm->onChanageSearch();
    });

    QSplitter * mainSplitter = new QSplitter(Qt::Vertical);

    mainSplitter->addWidget(ui->stackedWidget);
    mainSplitter->addWidget(ui->frame);

    mainSplitter->setStretchFactor(0, 4);
    mainSplitter->setStretchFactor(1, 1);

    mainSplitter->setOpaqueResize(false);
    //mainSplitter->setHandleWidth(3);
    //mainSplitter->setStyleSheet("QSplitter::handle{background:rgb(200, 0, 0)}");

    mainSplitter->show();

    ui->frame->hide();

    ui->verticalLayout->addWidget(mainSplitter);
}

void MainWindow::onSearch(Json::Value condition)
{
    //范围
    std::string scope = condition["scope"].asString();
    if(scope == "all")
    {
        for(auto uuid : mFlowWidgetManager.keys())
        {
            emit toSearch(uuid, condition);
        }
    }
    else
    {
        //当前
        emit toSearch(mCurFlowWidgetUuid, condition);
    }
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
    if(mRecordNavigationObj->isModify(mCurFlowWidgetUuid))
        emit saveProject(mRecordNavigationObj->curItem(mCurFlowWidgetUuid));
}

void MainWindow::onFlowChange(QString sysName, int sysType, QString testName, QString uuid, bool isNew)
{
//    //modify xqx 2020-6-19 21:07:59 每次都创建内存太大，容易内存不足，改为只有一个

//    if(mFlowWidgetManager.keys().size() > 0)
//    {
//        auto tmpPoint = mFlowWidgetManager.value(mFlowWidgetManager.keys()[0]);

//        delete tmpPoint;

//        tmpPoint = nullptr;

//        mFlowWidgetManager.remove(mFlowWidgetManager.keys()[0]);
//    }
//    //end

    //如果已有窗体则切换，否则新建窗体
    if(mFlowWidgetManager.contains(uuid))
    {
        //已有则切
        ui->stackedWidget->setCurrentWidget(mFlowWidgetManager[uuid]);

        dynamic_cast<flowTree*>(mFlowWidgetManager[uuid])->setSystemAndProductName(sysName, testName);
    }
    else
    {
        //新创建
        flowTree *flowWidget = new flowTree(uuid, sysType);

        flowWidget->setSystemAndProductName(sysName, testName);

        connect(this, &MainWindow::valueChange, flowWidget, &flowTree::onPropertyValueChange);

        connect(this, &MainWindow::saveProject, flowWidget, &flowTree::onSaveProject);
        connect(this, &MainWindow::showCurItemProperty, flowWidget, &flowTree::onShowCurItemProperty);
        connect(flowWidget, &flowTree::toShowProperty, this, &MainWindow::toShowProperty);
        connect(flowWidget, &flowTree::projectModify, mRecordNavigationObj, &recordNavigation::onProjectModify);
        connect(flowWidget, &flowTree::saveProjectOver, mRecordNavigationObj, &recordNavigation::onProjectAlreadySave);
        connect(this, &MainWindow::updateProject, flowWidget, &flowTree::onUpdateProject);
        connect(flowWidget, &flowTree::updateProperty, this, &MainWindow::updateProperty);
        connect(flowWidget, &flowTree::saveProjectOver, this, &MainWindow::onProjectAlreadySave);
        connect(flowWidget, &flowTree::singleTeset, mCmdDecodeObj.get(), &cmdDecode::onSendJson);
        connect(mRecordNavigationObj, &recordNavigation::frameTypeChange, flowWidget, &flowTree::onFrameTypeChange);
        connect(flowWidget, &flowTree::setGroupPropertyEnable, mPropertyWidgetObj, &propertyWidget::setGroupPropertyEnable);

        connect(flowWidget, &flowTree::removeProperty, mPropertyWidgetObj, &propertyWidget::removeProperty);
        connect(flowWidget, &flowTree::removeGroupProperty, mPropertyWidgetObj, &propertyWidget::removeGroupProperty);

        connect(flowWidget, &flowTree::addGroupProperty, mPropertyWidgetObj, &propertyWidget::addGroupProperty);
        connect(flowWidget, &flowTree::addProperty, mPropertyWidgetObj, &propertyWidget::addProperty);


        connect(flowWidget, &flowTree::searchResult, this, &MainWindow::searchResult);
        connect(this, &MainWindow::toSearch, flowWidget, &flowTree::onSearch);
        connect(this, &MainWindow::positionResult, flowWidget, &flowTree::onPositionResult);
        connect(mCmdDecodeObj.get(), &cmdDecode::testMsg, this, &MainWindow::onTestMsg);
        mFlowWidgetManager[uuid] = flowWidget;

        ui->stackedWidget->addWidget(flowWidget);
        ui->stackedWidget->setCurrentWidget(flowWidget);
    }    

    emit showCurItemProperty(uuid);

    //更新树(新创建的则不用更新)
    if(!isNew)
        emit updateProject(uuid);

    mCurFlowWidgetUuid = uuid;
}

void MainWindow::onShowMessage(QString msg)
{
    //this->statusBar()->showMessage(msg);
}

void MainWindow::onTestMsg(Json::Value msg)
{

}

void MainWindow::onPositionResult(QString projectUuid, QString itemUuid)
{
    if(mCurFlowWidgetUuid.compare(projectUuid) == 0)
    {

    }
    else
    {
        mRecordNavigationObj->onSwitchItem(projectUuid);
        if(mFlowWidgetManager.contains(projectUuid))
        {
            //已有则切
            ui->stackedWidget->setCurrentWidget(mFlowWidgetManager[projectUuid]);

            emit showCurItemProperty(projectUuid);

            //更新树(新创建的则不用更新)

            emit updateProject(projectUuid);

            mCurFlowWidgetUuid = projectUuid;
        }
    }

    emit positionResult(projectUuid, itemUuid);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(mRecordNavigationObj)
    {
        mSaveProjectUuid.clear();
        mSaveProjectUuid = mRecordNavigationObj->getModifyProject();
        if(mSaveProjectUuid.size() > 0)
        {
            if(QMessageBox::Yes == QMessageBox::warning(this, "提示", "有未保存的更改，是否保存",QMessageBox::Yes | QMessageBox::No))
            {
                isExit = true;

                foreach (QString uuid, mSaveProjectUuid)
                {
                    //保存
                    emit saveProject(mRecordNavigationObj->curItem(uuid));
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
