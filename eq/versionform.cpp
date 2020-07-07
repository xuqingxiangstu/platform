#include "versionform.h"
#include "ui_versionform.h"
#include <QDate>
#include <QTime>
#include <QDebug>

versionForm::versionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::versionForm)
{
    ui->setupUi(this);

    QString dataTime = "";
    dataTime += __DATE__;
    dataTime += " ";
    dataTime += __TIME__;

    QString tmp = dataTime.replace("  ", " ");

    QDateTime buildTime = QLocale(QLocale::English).toDateTime(tmp, "MMM d yyyy hh:mm:ss");

    QString version;
#ifndef QT_NO_DEBUG
    version = "V" + QString(VERSION) + "." + buildTime.toString("yyyyMMdd") + "_Debug";
#else
    version = "V" + QString(VERSION) + "." + buildTime.toString("yyyyMMdd") + "_Release";
#endif
    ui->label->setText(version);
}

versionForm::~versionForm()
{
    delete ui;
}
