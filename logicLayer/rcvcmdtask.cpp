#include "rcvcmdtask.h"
#include<QDateTime>
rcvCmdTask::rcvCmdTask(QObject *parent)
:QThread(parent),mZmqRcv(nullptr)
{       
}
void rcvCmdTask::run()
{
    char rcvBuf[1024] = {0};
    int rcvSize = 0;
 
     while(1)
    {
        if(mZmqRcv->receiveMsg(rcvBuf, rcvSize))
        {
            if(rcvSize > 0)
            {
                emit toParse(QString::fromUtf8((const char*)rcvBuf, rcvSize));
            }
        }
    }
}
