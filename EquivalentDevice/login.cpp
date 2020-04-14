#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QProgressDialog>
login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
}

login::~login()
{
    delete ui;
}
void login::on_loginBtn_clicked()
{
    if((ui->name->text()=="")||(ui->password->text()==""))
    {
        QMessageBox::information(this,tr("警告"),tr("输入不能为空！"),QMessageBox::Ok);
        return;
    }
    QString name= ui->name->text();
    QString passward=ui->password->text();

    QProgressDialog dialog(tr("正在登录"),tr("取消"),0,1000,this);
    dialog.setWindowTitle(tr("进度"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.show();
    for(int k=0;k<1000;k++)
        {
            dialog.setValue(k);
            QCoreApplication::processEvents();
            if(dialog.wasCanceled())
                {
                    break;
                }
        }
    dialog.setValue(1000);
    this->close();
    MainWindow *Win=new MainWindow;
    Win->show();
}
