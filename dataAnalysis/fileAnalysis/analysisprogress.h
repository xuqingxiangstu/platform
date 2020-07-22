#ifndef ANALYSISPROGRESS_H
#define ANALYSISPROGRESS_H

#include <QDialog>
#include <QMovie>

namespace Ui {
class analysisProgress;
}

class analysisProgress : public QDialog
{
    Q_OBJECT

public:
    explicit analysisProgress(QWidget *parent = 0);
    ~analysisProgress();
public slots:

    void setMinMaxV(int min, int max);

    void setCurV(int cur);

    void onClose();
private:
    QMovie *mMove;
private:
    Ui::analysisProgress *ui;
};

#endif // ANALYSISPROGRESS_H
