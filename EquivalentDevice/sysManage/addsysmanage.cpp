#include "addsysmanage.h"
#include "ui_addsysmanage.h"
#include "sysmanageui.h"
#include "../dbtableopt.h"
#include <QDebug>

addSysManage::addSysManage(QWidget *parent, QString mouseActive, QString devOldUuid, QString sysName, int changeRow) :
    QDialog(parent),
    ui(new Ui::addSysManage)
{
    Sparent=(sysManageUi*) parent;
    ui->setupUi(this);
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    ui->ipAddr->setValidator(new QRegExpValidator(rx));


    QRegExp regx("[0-9]+$");
    ui->ipPort->setValidator(new QRegExpValidator(regx));
    ui->groupBox_2->hide();
    ui->groupBox_3->hide();
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(QRect(150,0,this->width(),this->height()));

    m_mouseActive = mouseActive;
    m_devUuid = devOldUuid;
    m_sysName = sysName;
    m_changeRow = changeRow;
    ui->systemName->setText(m_sysName);
}

addSysManage::~addSysManage()
{
    delete ui;
}

void addSysManage::on_buttonBox_clicked(QAbstractButton *button)
{
    my_sys tran;
    tran.uuid = "";
    tran.sysName = "";
    tran.dev_type = "";
    tran.describe = "";

    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        Json::Value value;
        Json::Value item;
        QString devUuid;
        if(ui->systemName->text() == "" || ui->ipPort->text() == ""||ui->ipAddr->text() == "")
        {
            QMessageBox::information(this, tr("消息"), tr("内容不能为空!"), QMessageBox::Ok);
            return;
        }
        item["DEV_NAME"] = ui->systemName->text().toStdString();
        tran.sysName = ui->systemName->text();
        if(ui->UdpButton->isChecked()){
            value["DEV_TYPE"] = ui->UdpButton->text().toStdString();
            item["IP_ADDR"] = ui->ipAddr->text().toStdString();
            item["PORT"] = ui->ipPort->text().toStdString();
            item["RESERVE"] = "";
            tran.dev_type = ui->UdpButton->text();
            tran.describe = ui->ipAddr->text()+" : "+ui->ipPort->text();
        }else if(ui->UartButton->isChecked()){
            value["DEV_TYPE"] = ui->UartButton->text().toStdString();
            item["PORT_NUM"] = ui->comboBox->currentText().toStdString();
            item["BAUD_RATE"] = ui->comboBox_2->currentText().toStdString();
            item["DATA_BIT"] = ui->comboBox_3->currentText().toStdString();
            item["STOP_BIT"] = ui->comboBox_4->currentText().toStdString();
            item["PARITY_BIT"] = ui->comboBox_5->currentText().toStdString();
            item["RESERVE"] = "";
            //qDebug()<< ui->comboBox->currentText();
            tran.dev_type = ui->UartButton->text();
            tran.describe = ui->comboBox->currentText()+" : "+ui->comboBox_2->currentText()+" : "+ui->comboBox_3->currentText()+" : "+ui->comboBox_4->currentText()+" : "+ui->comboBox_5->currentText();

        }else if(ui->BButton->isChecked()){
            value["DEV_TYPE"] = ui->BButton->text().toStdString();
            item["CARD_NUM"] = ui->lineEdit_4->text().toStdString();
            item["MODE"] = ui->comboBox_6->currentText().toStdString();
            item["RT_ADDR"] = ui->lineEdit_6->text().toStdString();
            item["RESERVE"] = "";
            tran.dev_type = ui->BButton->text();
            tran.describe = ui->lineEdit_4->text()+" : "+ui->comboBox_6->currentText()+" : "+ui->lineEdit_6->text();
        }else if(ui->TcpButton->isChecked()){
            value["DEV_TYPE"] = ui->TcpButton->text().toStdString();
            item["IP_ADDR"] = ui->ipAddr->text().toStdString();
            item["PORT"] = ui->ipPort->text().toStdString();
            item["RESERVE"] = "";
            tran.dev_type = ui->TcpButton->text();
            tran.describe = ui->ipAddr->text()+" : "+ui->ipPort->text();
        }else if(ui->ServerTcpButton->isChecked()){
            value["DEV_TYPE"] = ui->ServerTcpButton->text().toStdString();
            item["IP_ADDR"] = ui->ipAddr->text().toStdString();
            item["PORT"] = ui->ipPort->text().toStdString();
            item["RESERVE"] = "";
            tran.dev_type = ui->ServerTcpButton->text();
            tran.describe = ui->ipAddr->text()+" : "+ui->ipPort->text();
        }
        value["DEVINFO"] = item;
        value["SYSTEM_UUID"]=addDevSysName.toStdString();
        value.toStyledString();
        if(m_mouseActive == "update"){
            if(DBTableOpt::getInstance()->deleteSystemDev(m_devUuid))
            {
                if(!DBTableOpt::getInstance()->addSystemDev(value,devUuid))
                {
                    QMessageBox::information(this, tr("消息"), tr("更新失败!"), QMessageBox::Ok);
                    return;
                }
                tran.uuid = devUuid;
                this->close();
                Sparent->changeUpdate(&tran,m_changeRow);
            }
        }else{

            if(!DBTableOpt::getInstance()->addSystemDev(value,devUuid))
            {
                QMessageBox::information(this, tr("消息"), tr("添加失败!"), QMessageBox::Ok);
                return;
            }

            tran.uuid = devUuid;
            this->close();
            Sparent->update(&tran);
        }
        return;
    }
    else if(ui->buttonBox->button(QDialogButtonBox::Close) == button)
    {
        this->close();
        Sparent->show();
    }
}



void addSysManage::on_comboBox_6_currentTextChanged(const QString &arg1)
{
    if(!QString::compare(arg1, "BC")){
        ui->lineEdit_6->setEnabled(false);
    }else{
        ui->lineEdit_6->setEnabled(true);
    }
}


void addSysManage::on_UdpButton_clicked()
{
    ui->groupBox->setTitle("UDP");
}

void addSysManage::on_TcpButton_clicked()
{
    ui->groupBox->setTitle("TCP");
}

void addSysManage::on_TcpButton_2_clicked()
{
    ui->groupBox->setTitle("ServerTcp");
    ui->groupBox->show();
    ui->groupBox_2->hide();
    ui->groupBox_3->hide();
}


