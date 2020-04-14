#ifndef EXPORTCMD_H
#define EXPORTCMD_H

#include <QDialog>
#include "ui_exportcmd.h"
#include "dbToExcel.h"

namespace Ui {
class exportCmd;
}
class exportCmd : public QDialog
{
	Q_OBJECT
public:
	enum enExportType{
		exportSure,
		exportExit
	};
public:
	exportCmd(QWidget *parent = 0);
	~exportCmd();
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
    Ui::exportCmd *ui;

	enExportType mExportType;

    dbToExcel *toExcel;

};

#endif // EXPORTCMD_H
