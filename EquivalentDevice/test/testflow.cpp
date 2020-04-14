#include "testflow.h"
#include "ui_testflow.h"
#include "../include/PfCommon/jsoncpp/json.h"
#include <QDebug>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QContextMenuEvent>
#include <QPalette>
#include <iostream>
#include <QMessageBox>
#include <QPushButton>
testflow::testflow(QWidget *parent,QString sysName,QString recUuid,QString flowName,bool flag,QStackedWidget *item) :
    QWidget(parent),
    mRunType(ALL_TEST),
    ui(new Ui::testflow)
{
    ui->setupUi(this);
    m_sysName = sysName;
    m_recUuid = recUuid;
    m_flowName = flowName;
    m_flag = flag;
    m_stackItem = item;

    ui->labelSubSysName->setText(m_sysName);
    ui->labelSubSysName->setStyleSheet("color:#2a4dfb;");

    QFont fontSys( "Microsoft YaHei", 14, 75);
    ui->labelSubSysName->setFont(fontSys);

    ui->labelTaskName->setText(m_flowName);
    ui->labelTaskName->setStyleSheet("color:#04fe00;");

    QFont fontTask( "Microsoft YaHei", 14, 75);
    ui->labelTaskName->setFont(fontTask);

    mRuningMovie = new QMovie(":/image/img/runing.gif.gif");
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_treeWidget_customContextMenuRequested(const QPoint &pos)));

    initBtn();
    onInitResultItem();
    ui->toolButtonStop->setEnabled(false);
}

testflow::~testflow()
{
    delete ui;
}

std::string testflow::getRunitems(const std::string &runmsg)
{
    Json::Value root;
    root["msgType"] = runmsg;
    root.toStyledString();
    std::string out = Json::FastWriter().write(root);
    return out;
}

std::string testflow::initRunitems(const std::vector<std::string> &record_uuid)
{
    Json::Value root;
    Json::Value item;
    Json::Value arrayObj;

    for(int i=0;i<record_uuid.size();i++){
        arrayObj.append(record_uuid[i]);
    }
    item["flows"] = arrayObj;
    root["msgType"] = "init_flow";
    root["msg"] = item;
    root.toStyledString();
    std::string out = Json::FastWriter().write(root);
    return out;
}
void testflow::testTreeShow(QJsonObject obj,QString msgType)
{
    ///检查流程类型

    QString recordUuid = obj["record_uuid"].toString();
    qDebug() << recordUuid;

    /*m_coverWidget = new QWidget(this,Qt::FramelessWindowHint);
    m_coverWidget->resize(m_stackItem->size());
    m_coverWidget->setObjectName("widget");// 设置对象句,相当于css里的id
    m_coverWidget->setStyleSheet("#widget {background-color:rgba(200, 200, 200,0.3);}");// 设置id对应元素的背景色
    m_coverWidget->show();*/

    if(recordUuid == m_recUuid){
        //if(!m_flag) return;
        if(msgType == "runitems"){

            setTreeItemsFirst(obj["flow"].toArray());
        }
        else if(msgType == "showMsg")
        {
            onShowMsg(obj["status"].toString().toStdString(), obj["show"].toString());

        }else if("testStatus" == msgType)
        {
            onTestFlow(obj["sub_flow_uuid"].toString(), obj["status"].toString());

        }else if("testOver" == msgType)
        {
            if(obj["result"].toBool() == true)
            {
                onShowMsg(CORRECT, "------测试已结束，测试通过!------");
                QMessageBox::information(this, QStringLiteral("提示"), "测试已结束，测试通过！");
            }
            else
            {
                onShowMsg(ERROR, "-- 测试已结束，存在错误，请查看测试报告！--");
                QMessageBox::information(this, QStringLiteral("提示"), "测试已结束，存在错误请查看测试报告！");
            }
            ui->toolButtonStart->setEnabled(true);
            ui->toolButtonStop->setEnabled(false);

        }else if(msgType == "manualTrigger"){
            QMessageBox::information(this, QStringLiteral("提示"), obj["result"].toString());
        }else if(msgType =="stop"){
            QMessageBox::information(this, QStringLiteral("提示"), obj["result"].toString());
        }else if(msgType == "start"){
            QMessageBox::information(this, QStringLiteral("提示"), obj["result"].toString());
        }else if(msgType == "init_flow" &&obj["result"].toString().compare("")!=0&&obj["result"].toString().compare("ok") !=0){
            QMessageBox::information(this, QStringLiteral("提示"), obj["result"].toString());
        }

    }else{

        return;
    }
}

void testflow::setTreeItemsFirst(QJsonArray obj)
{
    ui->treeWidget->clear();
    ui->treeWidget->setRootIsDecorated(true);
    QString flowDesName,uuid;
    for (int i = 0; i < obj.size(); i++)
    {
        QJsonObject jsonObject = obj[i].toObject();

        if (jsonObject.contains("describe"))
        {
            QJsonValue describeFlow = jsonObject.take("describe");
            if (describeFlow.isString())
            {
                flowDesName = describeFlow.toString();
            }
            QJsonValue uuidFlow = jsonObject.take("flow_uuid");
            if (uuidFlow.isString())
            {
                uuid = uuidFlow.toString();
            }
        }

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setData(flowName,Qt::UserRole,QVariant(uuid));
        item->setText(flowName,flowDesName);

        ui->treeWidget->addTopLevelItem(item);
        int h = 26;
        QLabel *lab = new QLabel(ui->treeWidget);
        lab->setScaledContents(true);
        lab->setFixedSize(QSize(h, h));

        ui->treeWidget->setItemWidget(item, testStatus, lab);

        setTreeItemsSecond(jsonObject.take("sub_flow").toArray(),item);

        mTreeItemSrcColor = item->background(0);
    }
    ui->treeWidget->expandAll();
}

void testflow::setTreeItemsSecond(QJsonArray obj,QTreeWidgetItem *item)
{

    QString subFlowDesName,trigger,delay,timing,dest_system,sub_flow_uuid,cmdOrParam;
    for (int i = 0; i < obj.size(); i++)
    {
        QTreeWidgetItem *itemSub = new QTreeWidgetItem();
        QJsonObject jsonObject = obj[i].toObject();
        if (jsonObject.contains("describe"))
        {
            QJsonValue describeSubFlow = jsonObject.take("describe");
            if (describeSubFlow.isString())
            {
                subFlowDesName = describeSubFlow.toString();
            }
            QJsonValue triggerSub = jsonObject.take("trigger");
            if (triggerSub.isString())
            {
                trigger = triggerSub.toString();
            }
            QJsonValue delaySub = jsonObject.take("delay");

            delay = QString::number(delaySub.toDouble(),10,0);


            QJsonValue timingSub = jsonObject.take("timing");
            if (timingSub.isString())
            {
                timing = timingSub.toString();
            }
            QJsonValue dest_systemSub = jsonObject.take("dest_system_name");
            if (dest_systemSub.isString())
            {
                dest_system = dest_systemSub.toString();
            }
            QJsonValue subFlowUuid = jsonObject.take("sub_flow_uuid");
            if (subFlowUuid.isString())
            {
                sub_flow_uuid = subFlowUuid.toString();
            }

            QJsonValue cmdOrParamV = jsonObject.take("cmdOrParam");
            if (cmdOrParamV.isString())
            {
                cmdOrParam = cmdOrParamV.toString();
            }
        }
        itemSub->setData(flowName,Qt::UserRole,QVariant(sub_flow_uuid));
        itemSub->setData(flowValue,Qt::UserRole,QVariant(""));
        itemSub->setData(triggerCon,Qt::UserRole,QVariant(trigger));
        itemSub->setData(flowDelay,Qt::UserRole,QVariant(delay));
        itemSub->setData(targetDev,Qt::UserRole,QVariant(dest_system));

        itemSub->setText(flowName,subFlowDesName);
        itemSub->setText(flowValue,"");
        itemSub->setText(triggerCon,trigger);
        itemSub->setText(flowDelay,delay);

        itemSub->setText(targetDev,dest_system);

        if(timing == "手动"){

        }else{
            itemSub->setData(sendTime,Qt::UserRole,QVariant(timing));
            itemSub->setText(sendTime,timing);
        }
        item->addChild(itemSub);

        int hSub = 26;
        QLabel *labSub = new QLabel(ui->treeWidget);
        labSub->setScaledContents(true);
        labSub->setFixedSize(QSize(hSub, hSub));

        if(timing == "手动"){
            m_manualTrigger = new QPushButton(ui->treeWidget);
            m_manualTrigger->setText(timing);
            m_manualTrigger->setFixedSize(QSize(45, 26));
            ui->treeWidget->setItemWidget(itemSub, sendTime, m_manualTrigger);
            connect(m_manualTrigger,SIGNAL(clicked(bool)), this,SLOT(slot_onClick()));
        }
        ui->treeWidget->setItemWidget(itemSub, testStatus, labSub);

        if(cmdOrParam == "param"){
            setTreeItemsThird(jsonObject.take("info").toArray(),itemSub);
        }
    }
}
void testflow::setTreeItemsThird(QJsonArray obj, QTreeWidgetItem *itemSub)
{

    QString infoname,infocoding,infovalue;
    for (int i = 0; i < obj.size(); i++)
    {
        QTreeWidgetItem *itemSubSub = new QTreeWidgetItem();
        QJsonObject jsonObject = obj[i].toObject();
        if (jsonObject.contains("name"))
        {
            QJsonValue subFlowInfoName = jsonObject.take("name");
            if (subFlowInfoName.isString())
            {
                infoname = subFlowInfoName.toString();
            }
            QJsonValue subFlowInfoCode = jsonObject.take("coding");
            infocoding = QString::number(subFlowInfoCode.toDouble(),10,0);
            QJsonValue subFlowInfoVal= jsonObject.take("value");
            infovalue = subFlowInfoVal.toString();
        }

        itemSubSub->setData(flowName,Qt::UserRole,QVariant(infovalue));

        itemSubSub->setText(flowName,infoname);

        itemSubSub->setData(flowValue,Qt::UserRole,QVariant(infovalue));
        itemSubSub->setText(flowValue,infovalue);
        itemSub->addChild(itemSubSub);

        int hSubSub = 26;
        QLabel *labSubSub = new QLabel(ui->treeWidget);
        labSubSub->setScaledContents(true);
        labSubSub->setFixedSize(QSize(hSubSub, hSubSub));

        ui->treeWidget->setItemWidget(itemSubSub, testStatus, labSubSub);


    }
}

void testflow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem  *curItem =  ui->treeWidget->itemAt(pos);
    if(curItem == NULL){
        return;
    }else if(!curItem->parent()){

    }else if(curItem->parent()->parent()){
        return;
    }
    testTree = new QMenu(ui->treeWidget);
    startTest = testTree->addAction("开始测试");
    connect(startTest, SIGNAL(triggered(bool)), this, SLOT(startflowTest()));
    testTree->move(cursor().pos());
    testTree->show();
}

void testflow::startflowTest()
{
    QTreeWidgetItem *currentItem = new QTreeWidgetItem();
    currentItem = ui->treeWidget->currentItem();
    Json::Value value;
    Json::Value item;

    value["msgType"] = "start";
    item["record_uuid"] = m_recUuid.toStdString();

    if(currentItem->parent()){
        item["flow_uuid"] = currentItem->parent()->data(flowName,Qt::UserRole).toString().toStdString();
        item["sub_flow_uuid"] = currentItem->data(flowName,Qt::UserRole).toString().toStdString();
    }else{
        item["flow_uuid"] = currentItem->data(flowName,Qt::UserRole).toString().toStdString();
        item["sub_flow_uuid"] = "";
    }
    value["msg"] = item;

    value.toStyledString();
    std::string out = Json::FastWriter().write(value);

    sendCmd(QString::fromLocal8Bit(out.c_str()));

    ui->toolButtonStart->setEnabled(false);
    ui->toolButtonStop->setEnabled(true);
    sendRecid(m_recUuid);
}

void testflow::onShowMsg(std::string status, QString msg)
{
    ui->textBrowserOutPut->setFocus();
    ui->textBrowserOutPut->moveCursor(QTextCursor::End);

    //移动滚动条到底部
    QScrollBar *scrollbar = ui->textBrowserOutPut->verticalScrollBar();
    if (scrollbar)
    {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }

    QTextCharFormat fmt;

    if(status == NORMAL)
        fmt.setForeground(QColor(143, 200, 216));
    else if(status == ERROR)
        fmt.setForeground(QColor(255, 0, 0));
    else if(status == CORRECT)
        fmt.setForeground(QColor(0, 255, 0));
    else
        fmt.setForeground(QColor(143, 200, 216));
    QTextCursor cursor = ui->textBrowserOutPut->textCursor();
    cursor.mergeCharFormat(fmt);
    ui->textBrowserOutPut->mergeCurrentCharFormat(fmt);

    ui->textBrowserOutPut->insertPlainText(msg + "\n");

    //emit toLog(msg.toUtf8() + "\n");
}

void testflow::onTestFlow(QString subFlowUuid, QString status)
{
    QTreeWidgetItem *item = NULL;
    QTreeWidgetItemIterator Itor(ui->treeWidget);

    QString text = status;

    QVariant var;

    while (*Itor)
    {
        if((*Itor)->childCount()>0){
            for(int i = 0;i<(*Itor)->childCount();i++){

                var = (*Itor)->child(i)->data(flowName, Qt::UserRole);
                if (var == QVariant(subFlowUuid))
                {
                    item = (*Itor)->child(i);
                    break;
                }
            }
        }
        ++Itor;
    }
    if (item != NULL)
    {
        int findClumn = 0;

        findClumn = testStatus;

        if (0 == text.compare("test"))
        {
            QLabel *lab = (QLabel*)(ui->treeWidget->itemWidget(item, findClumn));
            if (lab)
            {
                lab->setMovie(mRuningMovie);
                mRuningMovie->start();
            }

            QColor color(217, 217, 217);

            //设置高亮
            for (int i = 0; i < ui->treeWidget->columnCount(); i++)
            {
               // item->setBackground(i, color);
            }
        }
        else if (0 == text.compare("normal"))
        {
            QLabel *lab = (QLabel*)(ui->treeWidget->itemWidget(item, findClumn));


            if (lab)
            {
                /*if(row == -1)
                {
                    lab->setText("测试正常");
                    lab->setStyleSheet("QLabel{color:rgb(0, 255, 0)}");
                }
                else
                {*/
                    QPixmap img(QString::fromUtf8(":/image/img/pass.png"));
                    img.scaled(lab->size(), Qt::KeepAspectRatio);

                    lab->setPixmap(img);
                //}
            }

            //设置正常 状态

            //设置高亮
            for (int i = 0; i < ui->treeWidget->columnCount(); i++)
            {
                if (item->text(testStatus).compare("") != 0)
                {
                    continue;
                }
                //item->setBackground(i, mTreeItemSrcColor);
            }
        }
        else if (0 == text.compare("error"))
        {
            QLabel *lab = (QLabel*)(ui->treeWidget->itemWidget(item, findClumn));

            if (lab)
            {
                /*if(row == -1)
                {
                    lab->setText("测试异常");
                    lab->setStyleSheet("QLabel{color:rgb(255, 0, 0)}");
                }
                else
                {*/
                    QPixmap img(QString::fromUtf8(":/image/img/unpass.png"));
                    img.scaled(lab->size(), Qt::KeepAspectRatio);

                    lab->setPixmap(img);
                //}
            }

            //设置高亮
            for (int i = 0; i < ui->treeWidget->columnCount(); i++)
            {
                //item->setBackground(i, mTreeItemSrcColor);
            }
        }
        ui->treeWidget->setCurrentItem(item);
        ui->treeWidget->scrollToItem(item);
    }
}

void testflow::onInitResultItem()
{
    QTreeWidgetItemIterator Itor(ui->treeWidget);
    while (*Itor)
    {
        if (0 == (*Itor)->childCount())
        {
            QLabel *lab = (QLabel*)(ui->treeWidget->itemWidget(*Itor, testStatus));
            if (lab)
            {
                QPixmap img(QString::fromUtf8(":/image/img/unload.png"));
                img.scaled(lab->size(), Qt::KeepAspectRatio);

                lab->setPixmap(img);

                for (int i = 0; i < ui->treeWidget->columnCount(); i++)
                {
                    (*Itor)->setBackground(i, mTreeItemSrcColor);
                }
            }
        }
        else
        {
            QLabel *lab = (QLabel*)(ui->treeWidget->itemWidget(*Itor, testStatus));
            if(lab)
            {
                lab->setText("");
                lab->setStyleSheet("");
            }
        }
        ++Itor;
    }
}

void testflow::initBtn()
{
    connect(ui->toolButtonStart, &QToolButton::clicked, [=](){
        onTestStart();
        sendRecid(m_recUuid);
    });
    connect(ui->toolButtonStop, &QToolButton::clicked, [=](){
        onTestStop();
        stopRecid(m_recUuid);
    });
}

void testflow::onTestStart()
{
    //QString text = ui->startLabel->text();
    QString text = "开始";

    mRunType = ALL_TEST;

    if (text.compare(QStringLiteral("开始")) == 0)
    {
        onInitResultItem();
        sendCmd(converJson("start"));

    }

    ui->toolButtonStart->setEnabled(false);
    ui->toolButtonStop->setEnabled(true);
}

void testflow::onTestStop()
{
    ui->toolButtonStart->setEnabled(false);
    ui->toolButtonStop->setEnabled(false);

    sendCmd(converJson("stop"));


}

QString testflow::converJson(QString msgType)
{
    QString json;

    Json::Value value;
    Json::Value item;

    value["msgType"] = msgType.toStdString();
    item["record_uuid"] = m_recUuid.toStdString();
    value["msg"] = item;
    value.toStyledString();
    std::string out = Json::FastWriter().write(value);
    json = QString::fromLocal8Bit(out.c_str());

    return json;
}

void testflow::onTestOver(bool res)
{
    ui->toolButtonStart->setText("开始");
    ui->toolButtonStart->setEnabled(true);
    ui->toolButtonStop->setEnabled(false);
    if(!res)
    {
        onShowMsg(ERROR, "-- 所有测试已结束，存在错误，请查看测试报告！--");
        QMessageBox::information(this, QStringLiteral("提示"), "所有测试已结束，存在错误请查看测试报告！");
    }
    else
    {
        onShowMsg(CORRECT, "------所有测试已结束，测试通过!------");
        QMessageBox::information(this, QStringLiteral("提示"), "所有测试已结束，测试通过！");
    }
}

void testflow::slot_onClick()
{
    QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
    if(senderObj == nullptr)
    {
        return;
    }

    QTreeWidgetItem  *currentItem =ui->treeWidget->itemAt(QPoint(senderObj->parentWidget()->frameGeometry().x(),senderObj->parentWidget()->frameGeometry().y()));
    //成功获取到所在行对应列的数据

    Json::Value value;
    Json::Value item;

    value["msgType"] = "manualTrigger";
    item["record_uuid"] = m_recUuid.toStdString();
    item["flow_uuid"] = currentItem->parent()->data(flowName,Qt::UserRole).toString().toStdString();
    item["sub_flow_uuid"] = currentItem->data(flowName,Qt::UserRole).toString().toStdString();
    value["msg"] = item;

    value.toStyledString();
    std::string out = Json::FastWriter().write(value);
    sendCmd(QString::fromLocal8Bit(out.c_str()));
}
