#ifndef DSMUILTIMG_H
#define DSMUILTIMG_H

#include <QWidget>

namespace Ui {
class dsMuiltImg;
}

class dsMuiltImg : public QWidget
{
    Q_OBJECT

public:
    explicit dsMuiltImg(QString dbPath, QWidget *parent = 0);
    ~dsMuiltImg();

private:
    Ui::dsMuiltImg *ui;
};

#endif // DSMUILTIMG_H
