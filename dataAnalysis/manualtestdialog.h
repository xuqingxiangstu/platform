#ifndef MANUALTESTDIALOG_H
#define MANUALTESTDIALOG_H

#include <QDialog>
#include <memory>
#include "./argParse/argumentparse.h"

namespace Ui {
class manualTestDialog;
}

class manualTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit manualTestDialog(QWidget *parent = 0);
    ~manualTestDialog();
signals:
    void toParse(QString uuid, Json::Value param, QByteArray vaildMsg);
public slots:
    void onShowResult(QJsonObject);
    void onShowMessage(QString msg, bool state = true);
private:
    void clearResultTable();
private:
    /**
     * @brief showMessage   显示消息
     * @param msg           内容
     * @param state         状态(ture:正常颜色、false:异常颜色-红色)
     */
    void showMessage(QString msg, bool state);
private:
    std::shared_ptr<argumentParse> mArgParse;
private:
    Ui::manualTestDialog *ui;
};

#endif // MANUALTESTDIALOG_H
