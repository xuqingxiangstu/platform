#ifndef VERSIONFORM_H
#define VERSIONFORM_H

#include <QWidget>

namespace Ui {
class versionForm;
}

#define VERSION "1.1.0"

class versionForm : public QWidget
{
    Q_OBJECT

public:
    explicit versionForm(QWidget *parent = 0);
    ~versionForm();

private:
    Ui::versionForm *ui;
};

#endif // VERSIONFORM_H
