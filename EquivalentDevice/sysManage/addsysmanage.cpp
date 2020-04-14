#include "addsysmanage.h"
#include "ui_addsysmanage.h"
#include "sysmanageui.h"
#include "../dbtableopt.h"
#include <QDebug>

addSysManage::addSysManage(QWidget *parent) :
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
        }
        value["DEVINFO"] = item;
        value["SYSTEM_UUID"]=addDevSysName.toStdString();
        value.toStyledString();
        if(!DBTableOpt::getInstance()->addSystemDev(value,devUuid))
        {
            QMessageBox::information(this, tr("消息"), tr("添加失败!"), QMessageBox::Ok);
            return;
        }
        //QMessageBox::information(this, tr("消息"), tr("添加成功!"), QMessageBox::Ok);
        tran.uuid = devUuid;
        this->close();
        Sparent->update(&tran);
        return;
    }
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
    {
        this->close();
        QProgressDialog dialog(tr("正在返回主界面"),tr("取消"), 0, 100, this);
        dialog.setWindowTitle(tr("进度"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.show();
        for(int k = 0; k < 100; k++)
        {
            dialog.setValue(k);
            QCoreApplication::processEvents();
            if(dialog.wasCanceled())
            {
                break;
            }
        }
        dialog.setValue(100);
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
