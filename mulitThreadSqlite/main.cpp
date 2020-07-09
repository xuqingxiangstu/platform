#include <QCoreApplication>

#include "../src/PfSql/paramsTable/paramstable.h"
#include "../src/PfCommon/jsoncpp/json.h"
#include <QThread>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    for(int i = 0; i < 500; i++)
    {
        QThread *thread = new QThread();

        QObject::connect(thread, &QThread::started, [=](){

            while(1)
            {
                Json::Value tmpV;

                if(paramsTable::getInstance()->getValue("8d", "4", tmpV))
                {
                    //qDebug() << tmpV.toStyledString().c_str();
                }


                QThread::usleep(10);
            }
            thread->quit();
        });

        thread->start();
    }


    return a.exec();
}
