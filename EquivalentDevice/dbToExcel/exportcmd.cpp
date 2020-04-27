#include "exportcmd.h"
#include "ui_exportcmd.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QDir>
#include <QDebug>

exportCmd::exportCmd(QWidget *parent)
    : QDialog(parent),ui(new Ui::exportCmd)
{
    ui->setupUi(this);

    connect(ui->selBtn, SIGNAL(clicked()), this, SLOT(onOpen()));
    connect(ui->exportBtn, SIGNAL(clicked()), this, SLOT(onExport()));
    connect(ui->sureBtn, SIGNAL(clicked()), this, SLOT(onSure()));

	mExportType = exportExit;
    toExcel = new dbToExcel(this);
    connect(toExcel, SIGNAL(setEnd(QString)), this, SLOT(onEnd(QString)));
    connect(toExcel, SIGNAL(setRange(int, int)), this, SLOT(onSetRange(int, int)));
    connect(toExcel, SIGNAL(setCurValue(int)), this, SLOT(onSetCurValue(int)));
}

exportCmd::~exportCmd()
{
    if (toExcel)
	{
        delete toExcel;
        toExcel = NULL;
	}
}

void exportCmd::onEnd(QString title)
{
    ui->sureBtn->setEnabled(true);
    ui->exportBtn->setEnabled(true);
}

void exportCmd::onSure()
{
	mExportType = exportSure;
	accept();
}
void exportCmd::onOpen()
{
    QFileDialog fileDlg;
    fileDlg.setWindowTitle("Open file");
    fileDlg.setDirectory("./");
    fileDlg.setFilter(QDir::Dirs);
    fileDlg.setNameFilter("*.xlsx");
    if (fileDlg.exec() == QDialog::Accepted)
    {
        QString fileName = fileDlg.selectedFiles()[0];

        if (fileName != "")
        {
            ui->lineEdit->setText(fileName);
        }
        else
        {
            return ;
        }
    }
    else
    {
        return;
    }
    ui->exportBtn->setEnabled(true);
}
void exportCmd::onExport()
{
    QString strFilePath = ui->lineEdit->text();
    if (strFilePath == "")
    {
        QMessageBox::information(this, tr("消息"), tr("文件为空，请选择文件!"), QMessageBox::Ok);
        return;
    }

    int index = strFilePath.lastIndexOf('.');

    if (index != -1)
    {
        QString str = strFilePath.mid(index + 1, strFilePath.size());
        if (str != QStringLiteral("xlsx"))
        {
            QMessageBox::information(this, tr("消息"), tr("文件为空，请选择文件!"), QMessageBox::Ok);
            return;
        }
    }
    else
    {
            QMessageBox::information(this, tr("消息"), tr("文件为空，请选择文件!"), QMessageBox::Ok);
            return;
    }

    ui->sureBtn->setEnabled(false);
    ui->exportBtn->setEnabled(false);

    QString reportpath  = QDir::currentPath()+ "/cfgfile/template.xlsx";

    QFileInfo fileInfo(reportpath);
    if(fileInfo.isFile()){
        QFile file(strFilePath);
            QString name = file.fileName();
            qDebug()<<name;
            if(file.exists()){
                file.remove(name);
            }
            if(QFile::copy(reportpath,strFilePath)){
                qDebug()<<QStringLiteral("复制成功");
            }else
                qDebug()<<QStringLiteral("复制失败");

            toExcel->setExcelFile(strFilePath);
            toExcel->startToExcel();

    }else{
        QMessageBox::information(this, tr("消息"), tr("请添加模板文件!"), QMessageBox::Ok);
        return;
    }
}

void exportCmd::onSetRange(int minimum, int maximum)
{
    ui->progressBar->setRange(minimum, maximum);
}
void exportCmd::onSetCurValue(int value)
{
    ui->progressBar->setValue(value);
}
