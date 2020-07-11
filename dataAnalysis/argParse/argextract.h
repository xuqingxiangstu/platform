#ifndef ARGEXTRACT_H
#define ARGEXTRACT_H

#include "../icdAdapter/icdmanager.h"

#include <QObject>
#include <QJsonObject>

//#define PRINT_RESULT    0   //打印结果宏定义，屏蔽则不需要打印

/**
 * @brief The argExtract class  参数提取基类
 */

class argExtract : public QObject
{
public:
    argExtract(QObject *parent = 0):
        QObject(parent)
    {

    }

    ~argExtract(){}

public:
    /**
     * @brief extract   参数提取
     * @param otherParam    其它参数
     * @param frameObj  解析类
     * @param result    解析结果
     */
    virtual void extract(const QString &uuid, const Json::Value &otherParam, std::shared_ptr<PfIcdWorkBench::frameObj> frameObj, const Json::Value &result){}
};

#endif // ARGEXTRACT_H
