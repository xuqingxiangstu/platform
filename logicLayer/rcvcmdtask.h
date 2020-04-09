#ifndef RCVCMDTASK_H
#define RCVCMDTASK_H

#include <QThread>
#include "../src/PfBus/zeroMq/zmqreceive.h"
#include "../src/PfAdapter/adapter/Adapter.h"
using namespace Pf;
/**
* @brief The rcvCmdTask class
* 接收界面cmd任务线程
*/
class rcvCmdTask : public QThread
{
    Q_OBJECT
public:

    rcvCmdTask(QObject *parent = nullptr);
    ~rcvCmdTask()
    {
        if(isRunning())
        {
            quit();
            wait();
        }
    }
signals:
    /**
     * @brief recordSig    记录日志信号函数
     * @param msg 需要记录的数据
     * @return 无
     */
    void recordSig(QByteArray msg);
public:
    /**
     * @brief startTask    开启任务现场
     */
    void startTask()
    {
        if(!isRunning())
        {
            start();
        }
    }
    /**
     * @brief setRcvAdapter 设置接收适配器
     * @param object接收适配器的对象
     */
    void setRcvAdapter(PfAdapter::Adapter *object){mZmqRcv = object;}
private:
    /**
     * @brief qt的run 线程虚函数
     */
    void run();
signals:
    /**
     * @brief toParse 命令解析信号
     * @param jsStr  命令内容
     */
    void toParse(QString jsStr);
    /**
     * @brief sendLogMsg 发送日志信号
     * @param msg 日志内容
     */
    void sendLogMsg(QByteArray msg);
private:
   PfAdapter::Adapter *mZmqRcv; /// >接收命令适配器对象
};

#endif // RCVCMDTASK_H
