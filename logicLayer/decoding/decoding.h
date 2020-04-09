#ifndef DECODING_H
#define DECODING_H

#include <QThreadPool>
#include <QRunnable>
#include <memory>
#include <QObject>

#include "../src/PfCommon/jsoncpp/json.h"

/**
 * @brief The decodingPool class    数据解析池
 */


class decoding : public QObject, public QRunnable
{
    Q_OBJECT
public:
    decoding();
    ~decoding();
signals:
    /**
     * @brief result    解析结果
     * @param v     结果
     */
    void result(Json::Value v);
protected:
    void run();
};

class decodingPool : public QObject
{
    Q_OBJECT
public:
    decodingPool(QObject *parent = 0);
public slots:
    void decode(QString uuid, QString ptl, QByteArray msg);
signals:
    /**
     * @brief result    解析结果
     * @param v     结果
     */
    void result(Json::Value v);
private:
    std::shared_ptr<QThreadPool> mThreadPool;
};

#endif // DECODING_H
