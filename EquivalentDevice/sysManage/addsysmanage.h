#ifndef ADDSYSMANAGE_H
#define ADDSYSMANAGE_H

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>
#include "sysmanageui.h"

namespace Ui {
class addSysManage;
}

class addSysManage : public QDialog
{
    Q_OBJECT

public:
    explicit addSysManage(QWidget *parent = 0, QString mouseActive = "", QString devOldUuid = "", QString sysName = "",int changeRow = NULL);
    ~addSysManage();
signals:
    void update();    
private slots:

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_comboBox_6_currentTextChanged(const QString &arg1);

    void on_UdpButton_clicked();

    void on_TcpButton_clicked();

    void on_TcpButton_2_clicked();

private:
    Ui::addSysManage *ui;
    sysManageUi *Sparent;
    QString m_mouseActive;
    QString m_devUuid;
    QString m_sysName;
    int m_changeRow;
};


#endif // ADDSYSMANAGE_H
