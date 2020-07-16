#include "newprjdialog.h"
#include "ui_newprjdialog.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

newPrjDialog::newPrjDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newPrjDialog)
{
    ui->setupUi(this);

    connect(ui->cancelBtn, &QPushButton::clicked, [=](){
       done(QDialog::Rejected);
       close();
    });

    connect(ui->sureBtn, &QPushButton::clicked, [=](){
        mPrjName = ui->nameLineEdit->text();
        mPrjPath = ui->pathLineEdit->text();
        if(mPrjName.compare("") == 0)
        {
            QMessageBox::information(this, "提示", "请输入项目名称!");
            return ;
        }

        if(mPrjPath.compare("") == 0)
        {
            QMessageBox::information(this, "提示", "请输项目路径!");
            return ;
        }

        QDir dir(mPrjPath);
        if(!dir.exists())
        {
            QString info;
            info = "文件路径\"";
            info += mPrjPath;
            info += "\"不存在";
            QMessageBox::information(this, "提示", info);
            return ;
        }

       done(QDialog::Accepted);
       close();
    });

    connect(ui->pushButton, &QPushButton::clicked, [=](){
        QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "./",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
        ui->pathLineEdit->setText(path);

    });

    connect(ui->pathLineEdit, &QLineEdit::textChanged, [=](){

        QString path = ui->pathLineEdit->text();
        QDir dir(path);
        if(!dir.exists())
        {
            QString info;
            info = "文件路径\"";
            info += path;
            info += "\"不存在";
            ui->errorInfoLabel->setText(info);
            ui->errorInfoLabel->setStyleSheet("QLabel{background-color:rgb(255,0,0);}");  //设置样式表
        }
        else
        {
            ui->errorInfoLabel->setText("");
            ui->errorInfoLabel->setStyleSheet("");
        }
    });
}

newPrjDialog::~newPrjDialog()
{
    delete ui;
}
