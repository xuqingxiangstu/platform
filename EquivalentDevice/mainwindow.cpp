#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEventLoop>
#include <sstream>
#include <algorithm>
#include <functional>
#include <QThread>
#include <QWidget>
#include <QMap>
#include <QDir>
#include <QMessageBox>
#include <QDialog>
#include <QDebug>
#include <test/cmddecode.h>
#include "./sysManage/sysmanageui.h"
#include "./flow/dataselect.h"
#include "./flow/flow.h"
#include "./excelToDb/importcmd.h"
#include "./dbToExcel/exportcmd.h"
#include "./simulation/simulation.h"
#include "./sysManage/sysmanageui.h"
#include "dbtableopt.h"
#include "../include/PfCommon/cmdToJson/cmdtojson.h"
#include "../include/PfCommon/jsoncpp/json.h"
#include "../include/PfCommon/tools/ut_error.h"
#include <QJsonObject>

#include "delegate/qtreeitemdelegate.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mCmdDecode(nullptr),
    mPfAdaterObjs(nullptr),
    mLogicLayerProcess(nullptr)
{
    ui->setupUi(this);

    //启动后台程序
#ifndef DEBUG_LOGIC
    mLogicLayerProcess = new QProcess;
    mLogicLayerProcess->start("./logicLayer.exe");
    connect(mLogicLayerProcess, &QProcess::readyRead, this, &MainWindow::onLogicOutPut);
#endif
    try
    {
        loading();
        initFlowTree();
        initSystemTree();
    }
    catch(std::runtime_error err)
    {
        QMessageBox::warning(this, "提示", QString::fromStdString(err.what()));
        exit(0);
    }
    catch(Json::LogicError err)
    {
        QMessageBox::warning(this, "提示", QString::fromStdString(err.what()));
        exit(0);
    }
    catch(...)
    {
        QMessageBox::warning(this, "提示", QString::fromStdString("[ERROR]"));
        exit(0);
    }

    //initFlowCom();
    connect(ui->syslistWidget,SIGNAL(itemClicked(QListWidgetItem*)), this,SLOT(switchPage(QListWidgetItem*)));
    ui->stackedWidget->setCurrentIndex(2);
    m_flag = true;
    taskCount = 0;
    m_saveRecid.clear();

}
MainWindow::~MainWindow()
{
    unLoading();
    delete ui;
    exit(0);
}

void MainWindow::onLogicOutPut()
{
    qDebug() << QString::fromLocal8Bit(mLogicLayerProcess->readAll());
}

void MainWindow::reset()
{
    loadSimTest();
}
void MainWindow::on_TbSysManage_triggered()
{
    sysManageUi *sysManage = new sysManageUi;
    sysManage->setWindowModality(Qt::ApplicationModal);
    sysManage->show();
}

void MainWindow::on_TbFlow_triggered()
{
#if 0
    flow *fl = new flow;
    fl->setWindowModality(Qt::ApplicationModal);
    fl->show();
#endif
    QProcess::execute("./eq.exe");
}
void MainWindow::on_TbSimulation_triggered()
{
    simulation *sim = new simulation(this);
    sim->setWindowModality(Qt::ApplicationModal);
    sim->exec();

    if(sim->m_simTestflags){
        sim->getSimTest(m_value);
        delete sim;
        loadSimTest();
    }else{
        delete sim;
    }
}

void MainWindow::on_ExcelToDb_triggered()
{
    importCmd *excel = new importCmd;
    excel->setWindowModality(Qt::ApplicationModal);
    excel->show();
}
void MainWindow::on_TbExport_triggered()
{
    exportCmd *toExcel = new exportCmd;
    toExcel->setWindowModality(Qt::ApplicationModal);
    toExcel->show();
}
void MainWindow::initSystemTree()
{
    DBTableOpt::getInstance()->getRecTestNotes(m_value);
    loadSimTest();
}

void MainWindow::initFlowTree()
{
    std::vector<std::string> msg;
    msg.clear();
    for(int i=0;i<m_value["RECORD_UUID"].size();i++)
    {
        msg.push_back(m_value["RECORD_UUID"][i].asString());
    }
    std::string json = testflow::initRunitems(msg);
    emit sendCmd(QString::fromLocal8Bit(json.c_str()));

    std::string msgRun = "runitems";
    std::string json1 = testflow::getRunitems(msgRun);
    emit sendCmd(QString::fromLocal8Bit(json1.c_str()));

}

void MainWindow::loading()
{
    mPfAdaterObjs = new Pf::PfAdapter::PfAdapterManager();

    QString filePath = QDir::currentPath() + "/cfgfile/dev.xml";

    mPfAdaterObjs->init(filePath.toStdString());

    ///命令解析
    mCmdDecode = new cmdDecode(this);
    connect(this, &MainWindow::sendCmd, mCmdDecode, &cmdDecode::onSendCmd);
    connect(mCmdDecode, &cmdDecode::toCmdMsg, this, &MainWindow::cmdMsg);

    connect(mCmdDecode, &cmdDecode::initFalid, this, &MainWindow::setFlag);

    Pf::PfAdapter::Adapter *rcvObj = nullptr;
    mPfAdaterObjs->getAdapter("cmdRcv", &rcvObj);

    Pf::PfAdapter::Adapter *sendObj = nullptr;
    mPfAdaterObjs->getAdapter("cmdSend", &sendObj);

    mCmdDecode->setSendObj(sendObj);
    mCmdDecode->setRcvObj(rcvObj);
    mCmdDecode->startDecode();
}
void MainWindow::initFlowCom()
{
    if(m_value["SYSTEM_NAME"].size()>0){
        for(int i=0;i<m_value["SYSTEM_NAME"].size();i++){
            m_testFlow = new testflow(this,QString::fromStdString(m_value["SYSTEM_NAME"][i].asString()),QString::fromStdString(m_value["RECORD_UUID"][i].asString()),QString::fromStdString(m_value["FLOW_NAME"][i].asString()),m_flag,ui->stackedWidget);
            connect(m_testFlow, &testflow::sendCmd, this, &MainWindow::sendCmd);
            connect(this, &MainWindow::cmdMsg, m_testFlow, &testflow::testTreeShow);
            connect(m_testFlow, &testflow::sendRecid, this, &MainWindow::saveRecid);
            connect(m_testFlow, &testflow::stopRecid, this, &MainWindow::deleRecid);
            ui->stackedWidget->addWidget(m_testFlow);
        }
    }else{
        m_testFlow = new testflow(this);
        connect(m_testFlow, &testflow::sendCmd, this, &MainWindow::sendCmd);
        connect(this, &MainWindow::cmdMsg, m_testFlow, &testflow::testTreeShow);
        ui->stackedWidget->addWidget(m_testFlow);
    }

}
void MainWindow::unLoading()
{
    if(mCmdDecode)
    {
        delete mCmdDecode;
        mCmdDecode = nullptr;
    }

    if(mPfAdaterObjs)
    {
        delete mPfAdaterObjs;
        mPfAdaterObjs = nullptr;
    }

    if(mLogicLayerProcess)
    {
        if(mLogicLayerProcess->isOpen())
            mLogicLayerProcess->close();

        delete mLogicLayerProcess;
    }
}

// 切换页面
void MainWindow::switchPage(QListWidgetItem * item)
{
    //int nCount = ui->stackedWidget->count();
    //int nIndex = ui->stackedWidget->currentIndex();

    //QListWidgetItem *curritem = ui->syslistWidget->currentItem();

    m_resetRecord = item->data(Qt::UserRole).toString();
    qDebug() << m_resetRecord;

    int index = ui->syslistWidget->currentRow();
    qDebug()<< index;
    ui->stackedWidget->setCurrentIndex(index);

    std::vector<QString>::iterator ret = std::find(m_saveRecid.begin(), m_saveRecid.end(), m_resetRecord);
    if(ret == m_saveRecid.end()){
        ui->mainToolBar->setEnabled(true);
    }else{
        ui->mainToolBar->setDisabled(true);
    }

}

void MainWindow::loadSimTest()
{

    for(int i = ui->stackedWidget->count(); i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
    clearSysTree();
    for(int i = 0;i<m_value["SYSTEM_NAME"].size();i++)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(QString::fromStdString(m_value["SYSTEM_NAME"][i].asString()+"_"+m_value["FLOW_NAME"][i].asString()));
        item->setData(Qt::UserRole,QVariant(QString::fromStdString(m_value["RECORD_UUID"][i].asString())));
        //item->setText(0, QString::fromStdString(m_value["SYSTEM_NAME"][i].asString()+"_"+m_value["FLOW_NAME"][i].asString()));
        //item->setText(1, QString::fromStdString(m_value["RECORD_UUID"][i].asString()));
        //QTreeWidgetItem *itemSub = new QTreeWidgetItem(item);
        //item->addChild(itemSub);
        ui->syslistWidget->insertItem(i,item);

        //ui->sysTreeWidget->openPersistentEditor(itemSub);
        //QProgressBar *ro = new QProgressBar();
        //ro->setWindowTitle(QString::fromStdString(m_value["SYSTEM_NAME"][i].asString()+"_"+m_value["FLOW_NAME"][i].asString()));
        //ui->sysTreeWidget->setItemDelegate(new QtreeItemDelegate(ui->sysTreeWidget));
        //ui->sysTreeWidget->setItemWidget(itemSub,0,ro);
        //item->setBackground(0,QBrush(QPixmap(":/image/img/unpass.png")));

    }
    initFlowCom();
    initFlowTree();
}

void MainWindow::clearSysTree(){

    /*std::vector<int> idArray;
    QTreeWidgetItemIterator it(ui->sysTreeWidget);
    while (*it) {
        ui->sysTreeWidget->setCurrentItem(*it);
        QTreeWidgetItem *curritem = ui->sysTreeWidget->currentItem();
        int index = ui->sysTreeWidget->indexOfTopLevelItem(curritem);
        idArray.push_back(index);
        ++it;
     }
    sort(idArray.begin(), idArray.end(), less<int>());
    for(int i=idArray.size()-1;i>=0;i--){
        delete ui->sysTreeWidget->takeTopLevelItem(idArray[i]);
    }*/
    ui->syslistWidget->clear();

}

void MainWindow::on_resetSys_triggered()
{
    std::vector<std::string> msg;
    msg.clear();
    qDebug()<< m_resetRecord;
    msg.push_back(m_resetRecord.toStdString());
    std::string json = testflow::initRunitems(msg);
    emit sendCmd(QString::fromLocal8Bit(json.c_str()));

    std::string msgRun = "runitems";
    std::string json1 = testflow::getRunitems(msgRun);
    emit sendCmd(QString::fromLocal8Bit(json1.c_str()));
}

void MainWindow::setFlag(bool flag){
    m_flag = flag;
}

void MainWindow::saveRecid(QString recid)
{
    m_saveRecid.push_back(recid);

    ui->mainToolBar->setDisabled(true);
}

void MainWindow::deleRecid(QString recid)
{
    std::vector<QString>::iterator iter;

    iter = std::find(m_saveRecid.begin(),m_saveRecid.end(),recid);

    if(iter!=m_saveRecid.end())m_saveRecid.erase(iter);

    ui->mainToolBar->setEnabled(true);
}

