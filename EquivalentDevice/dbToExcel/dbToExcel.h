#ifndef DBTOEXCEL_H
#define DBTOEXCEL_H


#include <QThread>
#include <QSqlDatabase>

class dbToExcel : public QThread
{
	Q_OBJECT
public:
    enum frameStr{
        INDEX,
        CODING_NUM,
        FRAME_TYPE,
        INFO_WORD_TYPE,
        IS_ACK,
        CMD_TYPE,
        GROUP_NAME,
        PARAM_NAME,
        PARAM_BYTE_START_POS,
        PARAM_BYTE_SIZE,
        PARAM_BIT_START_POS,
        PARAM_BIT_SIZE,
        L_B_ENDIAN,
        INIT_VALUE,
        MIN_VALUE,
        MAX_VALUE,
        DATA_TYPE,
        SYSTEM,
        VALUE_MEAN,
    };
public:
    dbToExcel(QObject *parent);
    ~dbToExcel();
public:
    /**
     * @brief setExcelFile 获取需要导入的excel的路径
     * @param file
     */
	void setExcelFile(QString file){ strFilePath = file; }
    /**
     * @brief startToExcel 开线程,开始导入
     */
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

#endif // DBTOEXCEL_H
