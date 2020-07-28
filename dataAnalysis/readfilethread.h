#ifndef READFILETHREAD_H
#define READFILETHREAD_H

#include <QObject>
#include <QThread>

/**
 * @brief The readFileThread class  读取文件线程
 */

class readFileThread : public QThread
{
    Q_OBJECT
public:
    explicit readFileThread(QString path, QObject *parent = 0);

public:
    void startTask()
    {
        if(!isRunning())
        {
            start();
        }
    }
signals:
    void updateContext(QByteArray context);
public slots:

private:
    void run();
private:
    QString mFilePath;
};

#endif // READFILETHREAD_H
