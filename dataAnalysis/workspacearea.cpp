#include "workspacearea.h"
#include "ui_workspacearea.h"

workspaceArea::workspaceArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::workspaceArea)
{
    ui->setupUi(this);

    mDataShowTable = new dataShowTable();
    ui->verticalLayout_2->addWidget(mDataShowTable);
}

workspaceArea::~workspaceArea()
{
    delete ui;
}
