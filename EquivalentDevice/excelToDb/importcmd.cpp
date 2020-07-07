#include "importcmd.h"
#include "ui_importcmd.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QString>

importCmd::importCmd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importCmd)
{
    ui->setupUi(this);

    connect(ui->selBtn, SIGNAL(clicked()), this, SLOT(onOpen()));
    connect(ui->sureBtn, SIGNAL(clicked()), this, SLOT(onSure()));
    connect(ui->exportBtn, SIGNAL(clicked()), this, SLOT(onExport()));

    mExportType = exportExit;

    toDb = new excelToDb(this);

    connect(toDb, SIGNAL(setEnd(QString)), this, SLOT(onEnd(QString)));
    connect(toDb, SIGNAL(setRange(int, int)), this, SLOT(onSetRange(int, int)));
    connect(toDb, SIGNAL(setCurValue(int)), this, SLOT(onSetCurValue(int)));
}

importCmd::~importCmd()
{
    if (toDb)
    {
        delete toDb;
        toDb = NULL;
    }

    delete ui;
}

void importCmd::onEnd(QString title)
{
    ui->sureBtn->setEnabled(true);
    ui->selBtn->setEnabled(true);
    ui->exportBtn->setEnabled(true);
    //QMessageBox::information(this, QString::fromLocal8Bit("��Ϣ"), QString::fromLocal8Bit(title.toStdString().c_str()), QMessageBox::Ok);
}

void importCmd::onSure()
{
    mExportType = exportSure;
    accept();
}
void importCmd::onOpen()
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
void importCmd::onExport()
{
    QString strFilePath = ui->lineEdit->text();
    if (strFilePath == "")
    {
        QMessageBox::information(this, QString::fromLocal8Bit("��Ϣ"), QString::fromLocal8Bit("�ļ�Ϊ�գ���ѡ���ļ�!"), QMessageBox::Ok);
        return;
    }

    int index = strFilePath.lastIndexOf('.');

    if (index != -1)
    {
        QString str = strFilePath.mid(index + 1, strFilePath.size());
        if (str != QStringLiteral("xlsx"))
        {
            QMessageBox::information(this, QString::fromLocal8Bit("��Ϣ"), QString::fromLocal8Bit("�ļ�Ϊ�գ���ѡ���ļ�!"), QMessageBox::Ok);
            return;
        }
    }
    else
    {
            QMessageBox::information(this, QString::fromLocal8Bit("��Ϣ"), QString::fromLocal8Bit("�ļ�Ϊ�գ���ѡ���ļ�!"), QMessageBox::Ok);
            return;
    }

    toDb->setExcelFile(strFilePath);

    ui->sureBtn->setEnabled(false);
    ui->selBtn->setEnabled(false);
    ui->exportBtn->setEnabled(false);

    toDb->startToExcel();
}

void importCmd::onSetRange(int minimum, int maximum)
{
    ui->progressBar->setRange(minimum, maximum);
}
void importCmd::onSetCurValue(int value)
{
    ui->progressBar->setValue(value);
}

