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
#include "../../src/PfCommon/cmdToJson/cmdtojson.h"
#include "../../src/PfCommon/jsoncpp/json.h"
#include "../../src/PfCommon/tools/ut_error.h"
#include <QJsonObject>
#include <QScrollBar>
#include <QInputDialog>
#include <QPropertyAnimation>
#include <QDateTime>
#include <QTimer>
#include "delegate/qtreeitemdelegate.h"

using namespace std;

int startAllCnt = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mCmdDecode(nullptr),
    mPfAdaterObjs(nullptr),
    mLogicLayerProcess(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    m_changeFlag = false;
    m_errRecoid.clear();
    m_saveErrRecid.clear();
    ui->groupBox_2->show();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));

    timer->start(1000);


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
    QObject::connect(ui->closeButton,&QPushButton::clicked,[=](){
        exit(0);
    });
    //initFlowCom();
    connect(ui->syslistWidget,SIGNAL(itemClicked(QListWidgetItem*)), this,SLOT(switchPage(QListWidgetItem*)));
    ui->stackedWidget->setCurrentIndex(0);
    m_flag = true;
    taskCount = 0;
    m_saveRecid.clear();
    m_saveAllStartId.clear();
    this->setStyleSheet("#MainWindow{background-image:url(:/image/img/bg.png)}"
                        "#groupBox{border:1px solid #1e75ff;}");
    ui->syslistWidget->setStyleSheet("background-color:transparent;"
                                     "QListWidget::Item{padding-top:20px;padding-bottom:20px;}");
    ui->syslistWidget->setFrameShape(QListWidget::NoFrame);
    ui->pushButtonStopAll->setDisabled(true);
    ui->pushButtonStopAll->setStyleSheet("QPushButton{background: url(:/image/img/stopAN.png);border:none;outline:none;color:#fff;}");

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

            connect(m_testFlow, &testflow::sendErrRecid, this, &MainWindow::saveErrRecid);
            connect(m_testFlow, &testflow::stopErrRecid, this, &MainWindow::deleErrRecid);

            ui->stackedWidget->addWidget(m_testFlow);
            connect( m_testFlow, &testflow::initFlowErr,m_sysTreeItem, &qtreeitemdelegate::setErrTreeBg);

            connect(this, &MainWindow::sendAllStartId, m_testFlow, &testflow::rcvAllStart);
            connect(this, &MainWindow::sendStartSignal, m_testFlow, &testflow::setStartBtn);
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
    m_resetRecord = item->data(Qt::UserRole).toString();
    qDebug() << m_resetRecord;

    int index = ui->syslistWidget->currentRow();
    qDebug()<< index;
    ui->stackedWidget->setCurrentIndex(index);

    std::vector<QString>::iterator ret = std::find(m_saveRecid.begin(), m_saveRecid.end(), m_resetRecord);
    if(ret == m_saveRecid.end()){
        ui->groupBox_2->setEnabled(true);
        setBtnAble(true);
    }else{
        ui->groupBox_2->setDisabled(true);
        setBtnAble(false);
    }

    std::vector<QString>::iterator errRet = std::find(m_saveErrRecid.begin(), m_saveErrRecid.end(), m_resetRecord);
    if(errRet != m_saveErrRecid.end()){
        ui->pushButtonStartAll->setDisabled(true);
        ui->pushButtonStartAll->setStyleSheet("QPushButton{background: url(:/image/img/startAN.png);border:none;outline:none;color:#fff;}");

        ui->pushButtonStopAll->setDisabled(true);
        ui->pushButtonStopAll->setStyleSheet("QPushButton{background: url(:/image/img/stopAN.png);border:none;outline:none;color:#fff;}");
    }
    emit showCurrent(m_resetRecord);
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
        m_sysTreeItem = new qtreeitemdelegate(this,QString::fromStdString(m_value["SYSTEM_NAME"][i].asString()),QString::fromStdString(m_value["RECORD_UUID"][i].asString()),QString::fromStdString(m_value["FLOW_NAME"][i].asString()));
        item->setData(Qt::UserRole,QVariant(QString::fromStdString(m_value["RECORD_UUID"][i].asString())));
        item->setSizeHint(QSize(292,85));
        ui->syslistWidget->insertItem(i,item);
        ui->syslistWidget->setItemWidget(item,m_sysTreeItem);
        connect(this, &MainWindow::showCurrent, m_sysTreeItem, &qtreeitemdelegate::showCurrent);
        connect( m_sysTreeItem, &qtreeitemdelegate::sendToMain,this, &MainWindow::revRecid);

        connect(this, &MainWindow::sendErrId, m_sysTreeItem, &qtreeitemdelegate::setErrId);
    }
    m_resetRecord = QString::fromStdString(m_value["RECORD_UUID"][0].asString());
    emit showCurrent(QString::fromStdString(m_value["RECORD_UUID"][0].asString()));
    initFlowCom();
    initFlowTree();
}

void MainWindow::clearSysTree(){

    ui->syslistWidget->clear();

}
void MainWindow::setFlag(bool flag){
    m_flag = flag;
}

void MainWindow::saveRecid(QString recid)
{
    m_saveRecid.push_back(recid);

    ui->groupBox_2->setDisabled(true);
    setBtnAble(false);
}

void MainWindow::deleRecid(QString recid,int cnt)
{
    std::vector<QString>::iterator iter;

    iter = std::find(m_saveRecid.begin(),m_saveRecid.end(),recid);

    if(iter!=m_saveRecid.end())m_saveRecid.erase(iter);

    ui->groupBox_2->setEnabled(true);
    setBtnAble(true);



    if(cnt == m_saveAllStartId.size()){

        ui->pushButtonStopAll->setEnabled(false);
        ui->pushButtonStopAll->setStyleSheet("QPushButton{background: url(:/image/img/stopAN.png);border:none;outline:none;color:#fff;}");
        ui->pushButtonStartAll->setEnabled(true);
        ui->pushButtonStartAll->setStyleSheet("QPushButton{background: url(:/image/img/startA.png);border:none;outline:none;color:#fff;}");
    }
}

void MainWindow::on_simBtn_clicked()
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

void MainWindow::on_flowBtn_clicked()
{
    #if 0
        flow *fl = new flow;
        fl->setWindowModality(Qt::ApplicationModal);
        fl->show();
    #endif
        QProcess::startDetached("./eq.exe");
}

void MainWindow::on_sysBtn_clicked()
{
    sysManageUi *sysManage = new sysManageUi;
    sysManage->setWindowModality(Qt::ApplicationModal);
    sysManage->show();
}

void MainWindow::on_impBtn_clicked()
{
    importCmd *excel = new importCmd;
    excel->setWindowModality(Qt::ApplicationModal);
    excel->show();
}

void MainWindow::on_expBtn_clicked()
{
    exportCmd *toExcel = new exportCmd;
    toExcel->setWindowModality(Qt::ApplicationModal);
    toExcel->show();
}

void MainWindow::on_resetBtn_clicked()
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


void MainWindow::on_changeBtn_clicked()
{
    if(!m_changeFlag){
        bool isOK;
        QString systemName = QInputDialog::getText(NULL, "切换模式","请输入密码！", QLineEdit::Normal,"",&isOK);

        if(isOK&&systemName == "123456") {
            m_changeFlag = true;
            ui->groupBox_2->show();
        }else if(isOK&&systemName != "123456"){
            QMessageBox::warning(this, "提示", QString::fromStdString("密码错误！"));
        }

    }else{
        m_changeFlag = false;
        ui->groupBox_2->hide();
    }
}

void MainWindow::timeUpdate(void)
{
    QLabel *dataLabel = ui->dataTime;
    QDateTime *dataTime = new QDateTime(QDateTime::currentDateTime());
    QString str = dataTime->toString("yyyy/MM/dd  hh:mm:ss");
    dataLabel->setText(str);
    dataLabel->show();
}

void MainWindow::setBtnAble(bool flag)
{
    if(flag)//按钮使能
    {
        ui->sysBtn->setStyleSheet("background-image:url(:/image/img/system.png);border-radius:18px;color:#fff;");
        ui->simBtn->setStyleSheet("background-image:url(:/image/img/sim.png);border-radius:18px;color:#fff;");
        ui->flowBtn->setStyleSheet("background-image:url(:/image/img/flow.png);border-radius:18px;color:#fff;");
        ui->impBtn->setStyleSheet("background-image:url(:/image/img/importTb.png);border-radius:18px;color:#fff;");
        ui->resetBtn->setStyleSheet("background-image:url(:/image/img/reset.png);border-radius:18px;color:#fff;");
        ui->expBtn->setStyleSheet("background-image:url(:/image/img/exportTb.png);border-radius:18px;color:#fff;");

    }else{//按钮变灰
        ui->sysBtn->setStyleSheet("background-image:url(:/image/img/systemN.png);border-radius:18px;color:#fff;");
        ui->simBtn->setStyleSheet("background-image:url(:/image/img/simN.png);border-radius:18px;color:#fff;");
        ui->flowBtn->setStyleSheet("background-image:url(:/image/img/flowN.png);border-radius:18px;color:#fff;");
        ui->impBtn->setStyleSheet("background-image:url(:/image/img/importTbN.png);border-radius:18px;color:#fff;");
        ui->resetBtn->setStyleSheet("background-image:url(:/image/img/resetN.png);border-radius:18px;color:#fff;");
        ui->expBtn->setStyleSheet("background-image:url(:/image/img/exportTbN.png);border-radius:18px;color:#fff;");
    }
}

void MainWindow::revRecid(QString id,bool flag)
{
    if(flag){
        m_saveAllStartId.push_back(id);
    }else{
        std::vector<QString>::iterator iter;

        iter = std::find(m_saveAllStartId.begin(),m_saveAllStartId.end(),id);

        if(iter!=m_saveAllStartId.end())m_saveAllStartId.erase(iter);
    }
}

void MainWindow::on_pushButtonStartAll_clicked()
{
    QString msg = "start";
    ui->pushButtonStartAll->setDisabled(true);
    ui->pushButtonStartAll->setStyleSheet("QPushButton{background: url(:/image/img/startAN.png);border:none;outline:none;color:#fff;}");
    startAllCnt = m_saveAllStartId.size();
    for(int i=0;i<m_saveAllStartId.size();i++){
        qDebug() << m_saveAllStartId[i];
        sendAll(m_saveAllStartId[i],msg);
    }
    ui->pushButtonStopAll->setEnabled(true);
    ui->pushButtonStopAll->setStyleSheet("QPushButton{background: url(:/image/img/stopA.png);border:none;outline:none;color:#fff;}");

    m_AllStartRecoid = QVector<QString>::fromStdVector(m_saveAllStartId);

    emit sendAllStartId(m_AllStartRecoid);
}

void MainWindow::on_pushButtonStopAll_clicked()
{
    QString msg = "stop";
    ui->pushButtonStartAll->setDisabled(false);
    ui->pushButtonStartAll->setStyleSheet("QPushButton{background: url(:/image/img/startA.png);border:none;outline:none;color:#fff;}");
    for(int i=0;i<m_saveAllStartId.size();i++){
        sendAll(m_saveAllStartId[i],msg);
        emit sendStartSignal(m_saveAllStartId[i],msg);
    }
    ui->pushButtonStopAll->setDisabled(true);
    ui->pushButtonStopAll->setStyleSheet("QPushButton{background: url(:/image/img/stopAN.png);border:none;outline:none;color:#fff;}");
}

void MainWindow::saveErrRecid(QString recid)
{
    if(recid == m_resetRecord){
        ui->pushButtonStartAll->setDisabled(true);
        ui->pushButtonStartAll->setStyleSheet("QPushButton{background: url(:/image/img/startAN.png);border:none;outline:none;color:#fff;}");
    }
    m_saveErrRecid.push_back(recid);
    m_errRecoid = QVector<QString>::fromStdVector(m_saveErrRecid);
    emit sendErrId(m_errRecoid);
}

void MainWindow::deleErrRecid(QString recid)
{
    std::vector<QString>::iterator iter;

    iter = std::find(m_saveErrRecid.begin(),m_saveErrRecid.end(),recid);

    if(iter!=m_saveErrRecid.end())m_saveErrRecid.erase(iter);

    m_errRecoid = QVector<QString>::fromStdVector(m_saveErrRecid);

    emit sendErrId(m_errRecoid);
}

void MainWindow::sendAll(QString record_uuid,QString msgType)
{
    QString json;

    Json::Value value;
    Json::Value item;

    value["msgType"] = msgType.toStdString();
    item["record_uuid"] = record_uuid.toStdString();
    value["msg"] = item;
    value.toStyledString();
    std::string out = Json::FastWriter().write(value);
    json = QString::fromLocal8Bit(out.c_str());
    emit sendCmd(json);
}
