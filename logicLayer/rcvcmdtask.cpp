#include "rcvcmdtask.h"
#include<QDateTime>
#include <QDebug>

rcvCmdTask::rcvCmdTask(QObject *parent)
:QThread(parent),mZmqRcv(nullptr)
{       
}
void rcvCmdTask::run()
{
    const int maxSize = 20480;
    char rcvBuf[maxSize] = {0};
    int rcvSize = 0;
 
     while(1)
    {
        if(mZmqRcv->receiveMsg(rcvBuf, rcvSize,maxSize ))
        {
            if(rcvSize > 0)
            {                
                emit toParse(QString::fromUtf8((const char*)rcvBuf, rcvSize));
            }
        }
    }
}
