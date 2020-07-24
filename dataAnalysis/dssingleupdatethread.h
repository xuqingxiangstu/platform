#ifndef DSSINGLEUPDATETHREAD_H
#define DSSINGLEUPDATETHREAD_H

#include <QObject>

#include <QThread>

/**
 * @brief The dssingleUpdateThread class    单图更新线程
 */
class dssingleUpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit dssingleUpdateThread(QString dbPath, QString uuid, QObject *parent = 0);
public:
    void startTask()
    {
        if(!isRunning())
        {
            start();
        }
    }

signals:
    void updateTableNode(QString uuid, QString table);

    void updateCodingNode(QString fatherUuid, QString coding, QString name);
public slots:
private:
    void run();
private:
    QString mDbPath;
    QString mUuid;
};

#endif // DSSINGLEUPDATETHREAD_H
