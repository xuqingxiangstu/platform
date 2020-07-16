#ifndef NEWPRJDIALOG_H
#define NEWPRJDIALOG_H

#include <QDialog>

namespace Ui {
class newPrjDialog;
}

class newPrjDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newPrjDialog(QWidget *parent = 0);
    ~newPrjDialog();
public:
    /**
     * @brief getPrjName    获取项目名称
     * @return
     */
    QString getPrjName(){return mPrjName;}

    /**
     * @brief getPrjPath    获取项目路径
     * @return
     */
    QString getPrjPath(){return mPrjPath;}
private:
    QString mPrjName;
    QString mPrjPath;
private:
    Ui::newPrjDialog *ui;
};

#endif // NEWPRJDIALOG_H
