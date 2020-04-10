#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"

#include <QMessageBox>

newProjectDialog::newProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newProjectDialog)
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
            mProjectName = ui->lineEdit->text();
            mProjectDescribe = ui->textEdit->toPlainText();
            QDialog::accept();
        }
    });

    connect(ui->cancleBtn, &QPushButton::clicked, [=](){
        setResult(QDialog::Rejected);
       QDialog::reject();
    });
}

newProjectDialog::~newProjectDialog()
{
    delete ui;
}
