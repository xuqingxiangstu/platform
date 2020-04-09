#include "decoding.h"


decodingPool::decodingPool(QObject *parent):
    QObject(parent)
{
    mThreadPool = std::make_shared<QThreadPool>();
    mThreadPool->setMaxThreadCount(4);  //最大线程数
    mThreadPool->setExpiryTimeout(-1);
}

void decodingPool::decode(QString uuid, QString ptl, QByteArray msg)
{
    decoding *obj = new decoding();

    connect(obj, &decoding::result, this, &decodingPool::result);

    mThreadPool->start(obj);
}

/****************************************************/

decoding::decoding()
{

}

decoding::~decoding()
{

}

void decoding::run()
{

}
