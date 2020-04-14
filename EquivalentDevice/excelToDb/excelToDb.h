#ifndef EXCELTODB_H
#define EXCELTODB_H


#include <QThread>
#include <QSqlDatabase>

class excelToDb : public QThread
{
	Q_OBJECT

public:
    excelToDb(QObject *parent);
    ~excelToDb();
public:
	void setExcelFile(QString file){ strFilePath = file; }
	void startToExcel (){ start(); }
signals:
	void setEnd(QString);
	void setRange(int minimum, int maximum);
	void setCurValue(int value);
private:
	void run();
private:
	QString strFilePath;
};

#endif // EXCELTODB_H
