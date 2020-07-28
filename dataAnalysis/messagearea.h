#ifndef MESSAGEAREA_H
#define MESSAGEAREA_H

#include <QWidget>
#include <QMenu>

namespace Ui {
class messageArea;
}

class messageArea : public QWidget
{
    Q_OBJECT

public:
    explicit messageArea(QWidget *parent = 0);
    ~messageArea();
public slots:
    /**
     * @brief showMessage   显示消息
     * @param msg           内容
     * @param state         状态(ture:正常颜色、false:异常颜色-红色)
     */
    void showMessage(QString msg, bool state);
private:
    QMenu *mPopMenu;
    Ui::messageArea *ui;
};

#endif // MESSAGEAREA_H
