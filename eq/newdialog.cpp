#include "newdialog.h"
#include "ui_newdialog.h"

#include <QMessageBox>

newDialog::newDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newDialog)
{
    ui->setupUi(this);

    connect(ui->sureBtn, &QPushButton::clicked, [=](){

        if(ui->lineEdit->text().compare("") == 0)
        {
            QMessageBox::information(this, "提示", "请输入信息");
            return ;
        }
        else
        {
            QDialog::accept();
        }
    });

    connect(ui->cancleBtn, &QPushButton::clicked, [=](){
        setResult(QDialog::Rejected);
       QDialog::reject();
    });
}

void newDialog::setTitleText(QString txt)
{
    setWindowTitle(txt);
}

newDialog::~newDialog()
{
    delete ui;
}

QString newDialog::getDes()
{
    return ui->lineEdit->text();
}
