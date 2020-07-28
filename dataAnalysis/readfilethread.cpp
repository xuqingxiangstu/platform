#include "readfilethread.h"

#include <QFile>

readFileThread::readFileThread(QString path, QObject *parent)
    : QThread(parent),
      mFilePath(path)
{

}

void readFileThread::run()
{
    QFile fileObj(mFilePath);
    if(fileObj.open(QIODevice::ReadOnly))
    {
        while(1)
        {
            QByteArray msg;

            msg = fileObj.readLine();
            if(msg.isEmpty())
            {
                break;
            }
            else
            {
                emit updateContext(msg);
            }
            usleep(0);
        }
    }
}
