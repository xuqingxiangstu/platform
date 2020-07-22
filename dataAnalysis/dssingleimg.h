#ifndef DSSINGLEIMG_H
#define DSSINGLEIMG_H

#include <QWidget>

namespace Ui {
class dsSingleImg;
}

class dsSingleImg : public QWidget
{
    Q_OBJECT

public:
    explicit dsSingleImg(QString dbPath, QWidget *parent = 0);
    ~dsSingleImg();

private:
    Ui::dsSingleImg *ui;
};

#endif // DSSINGLEIMG_H
