#include "qtreeitemdelegate.h"
#include "ui_qtreeitemdelegate.h"
#include <QPainter>
#include <QDebug>

qtreeitemdelegate::qtreeitemdelegate(QWidget *parent,QString sysName,QString recUuid,QString flowName) :
    QWidget(parent),
    ui(new Ui::qtreeitemdelegate)
{
    ui->setupUi(this);
    m_sysName = sysName;
    m_recUuid = recUuid;
    m_flowName = flowName;
    initTreeItem();
    //this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeBg.png repeat center;)}");
    this->setAutoFillBackground(true);
    ui->sysName->setStyleSheet("color:#ffffff");
    m_errId.clear();
}

qtreeitemdelegate::~qtreeitemdelegate()
{
    delete ui;
}

void qtreeitemdelegate::initTreeItem()
{
    //ui->sysName->setText(m_sysName+"_"+m_flowName);
    ui->sysName->setText(m_flowName);
}

void qtreeitemdelegate::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void qtreeitemdelegate::showCurrent(QString recid)
{
        m_checkUuid = recid;
        if(m_errId.size()>0){
            for (int i = 0; i < m_errId.size(); ++i) {
                  if (m_errId.at(i) == m_recUuid){
                        m_flag = true;
                  }
            }
        }else{
            m_flag = false;
        }
        if(m_flag){
            this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeBgErr.png);}");
            if(m_recUuid == recid)
            {
                ui->clickCurrent->show();
            }else{
                ui->clickCurrent->hide();
            }
        }else{
            if(m_recUuid == recid)
            {
                this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeBg.png);}");
                ui->clickCurrent->show();

            }else if(m_recUuid != recid&&ui->checkBox->checkState() == Qt::Checked){
                ui->clickCurrent->hide();
                this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeBgCh.png);}");
            }else if(m_recUuid != recid&&ui->checkBox->checkState() != Qt::Checked){
                ui->clickCurrent->hide();
                this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeUnBg.png);}");
            }
        }

}

void qtreeitemdelegate::on_checkBox_stateChanged(int arg1)
{
    if(m_recUuid == m_checkUuid)
    {
        ui->clickCurrent->show();
        this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeBg.png);}");
    }else if(m_recUuid != m_checkUuid&&arg1 == Qt::Checked){
        ui->clickCurrent->hide();
        this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeBgCh.png);}");
    }else if(m_recUuid != m_checkUuid&&arg1 != Qt::Checked){
        ui->clickCurrent->hide();
        this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeUnBg.png);}");
    }

    if(arg1 == Qt::Checked)
    {
        emit sendToMain(m_recUuid, true);
    }else{
        emit sendToMain(m_recUuid, false);
    }
}
void qtreeitemdelegate::setErrTreeBg(QString recoid,bool flag)
{
    if(recoid == m_recUuid){
        if(flag){
           ui->checkBox->setDisabled(true);
           this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeBgErr.png);}");
        }else{
           if(m_checkUuid == m_recUuid){
              this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeBg.png);}");
           }else{
              this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeUnBg.png);}");
           }
           ui->checkBox->setEnabled(true);
        }
    }

}
void qtreeitemdelegate::setErrId(QVector<QString> &msg)
{
    for (int i = 0; i < msg.size(); ++i) {
          m_errId.push_back(msg[i]);
    }

    for (int i = 0; i < m_errId.size(); ++i) {
          if (m_errId.at(i) == m_recUuid){
              ui->checkBox->setDisabled(true);
              this->setStyleSheet("#qtreeitemdelegate{border-image:url(:/image/img/itemTreeBgErr.png);}");
          }
    }
}
