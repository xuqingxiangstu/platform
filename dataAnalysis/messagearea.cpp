#include "messagearea.h"
#include "ui_messagearea.h"

messageArea::messageArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::messageArea)
{
    ui->setupUi(this);
}

messageArea::~messageArea()
{
    delete ui;
}


void messageArea::showMessage(QString msg, bool state)
{

}
