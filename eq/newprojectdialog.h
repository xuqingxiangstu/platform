#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class newProjectDialog;
}

class newProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newProjectDialog(QWidget *parent = 0);
    ~newProjectDialog();
    QString name(){return mProjectName;}
    QString describe(){return mProjectDescribe;}

private:
    Ui::newProjectDialog *ui;
    QString mProjectName;
    QString mProjectDescribe;
};

#endif // NEWPROJECTDIALOG_H
