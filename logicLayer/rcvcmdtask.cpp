#include "rcvcmdtask.h"
#include<QDateTime>
rcvCmdTask::rcvCmdTask(QObject *parent)
:QThread(parent),mZmqRcv(nullptr)
{
     std::string Name = "CmdInteraction";
     mRecord =std::make_shared<PfCommon::RecordLog>(false);
     mRecord->setSource(Name);
     connect(this, &rcvCmdTask::recordSig, mRecord.get(), &Pf::PfCommon::RecordLog::record);
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

            ///TODO：存储日志

            QString timeStamp = QString(QDateTime::currentDateTime().toString("hh.mm.ss.zzz"));
            QString logTime = "------------" + timeStamp + "------------\n";
            QByteArray logTimeBy = logTime.toLatin1();
            QByteArray msg((const char*)rcvBuf, (int)rcvSize);
            QByteArray logMsg;
            logMsg.append(logTimeBy);
            logMsg.append(msg);
            logMsg.append("\n");
            emit sendLogMsg(logMsg);

        }
    }
}
