#ifndef QTREEITEMDELEGATE_H
#define QTREEITEMDELEGATE_H

#include <QWidget>
#include <QString>
#include <QVector>

namespace Ui {
class qtreeitemdelegate;
}

class qtreeitemdelegate : public QWidget
{
    Q_OBJECT

public:
    explicit qtreeitemdelegate(QWidget *parent = 0, QString sysName = "", QString recUuid = "", QString flowName = "");
    ~qtreeitemdelegate();
private:
    void initTreeItem();
signals:
    void sendToMain(QString,bool);
public slots:
    void showCurrent(QString recid);
    void setErrTreeBg(QString recoid, bool flag);
    void setErrId(QVector<QString> &msg);
    void setChecked(bool flag);
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void on_checkBox_stateChanged(int arg1);
private:
    Ui::qtreeitemdelegate *ui;

    QString m_sysName;
    QString m_recUuid;
    QString m_flowName;
    bool m_flag;
    bool m_isAllSelectflag;
    QString m_checkUuid;
    QVector<QString> m_errId;

};

#endif // QTREEITEMDELEGATE_H
