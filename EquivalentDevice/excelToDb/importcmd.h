#ifndef IMPORTCMD_H
#define IMPORTCMD_H

#include <QDialog>
#include "excelToDb.h"

namespace Ui {
class importCmd;
}

class importCmd : public QDialog
{
    Q_OBJECT
public:
    enum enExportType{
        exportSure,
        exportExit
    };
public:
    explicit importCmd(QWidget *parent = 0);
    ~importCmd();
public:
    enExportType getExportType(){ return mExportType; }
private slots:
    void onSure();
    void onOpen();
    void onExport();

    void onSetRange(int minimum, int maximum);
    void onSetCurValue(int value);

    void onEnd(QString);
private:
    Ui::importCmd *ui;

    enExportType mExportType;

    excelToDb *toDb;
};

#endif // IMPORTCMD_H
