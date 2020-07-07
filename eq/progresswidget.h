#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QDialog>
#include <QWidget>
namespace Ui {
class progressWidget;
}

class progressWidget : public QDialog
{
    Q_OBJECT

public:
    explicit progressWidget(QWidget *parent = 0);
    ~progressWidget();
public slots:
    void onClose();
private:
    QMovie *mMove;
private:
    Ui::progressWidget *ui;
};

#endif // PROGRESSWIDGET_H
