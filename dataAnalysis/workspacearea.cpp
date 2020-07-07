#include "workspacearea.h"
#include "ui_workspacearea.h"

workspaceArea::workspaceArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::workspaceArea)
{
    ui->setupUi(this);
}

workspaceArea::~workspaceArea()
{
    delete ui;
}
