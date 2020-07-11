#ifndef ARGUMENTPARSE_H
#define ARGUMENTPARSE_H

#include <QObject>
#include <QMap>

#include <memory>

#include "../src/PfCommon/jsoncpp/json.h"

#include "argextract.h"

/**
 * @brief The argumentParse class   参数解析类
 */

class argumentParse : public QObject
{
    Q_OBJECT
public:
    explicit argumentParse(QObject *parent = 0);
public:

signals:
    /**
     * @brief showMessage   显示消息提示
     * @param msg           消息
     * @param state         消息状态
     * -true:正常
     * -false:异常
     */
    void showMessage(QString msg, bool state);

    /**
     * @brief toDataBase    存储数据库
     * @param value         数据
     */
    void toDataBase(QJsonObject value);
public slots:
    /**
     * @brief parse     参数解析
     * @param param     参数
     * @param validMsg  有效数据
     */
    void parse(QString uuid, Json::Value param, QByteArray validMsg);
private:
    /**
     * @brief feParamExtract    FE帧参数提取
     * @param result
     */
    void feParamExtract(std::shared_ptr<PfIcdWorkBench::frameObj> obj, const Json::Value &result);

    /**
     * @brief beParamExtract    BE帧参数提取
     * @param result
     */
    void beParamExtract(std::shared_ptr<PfIcdWorkBench::frameObj> obj, const Json::Value &result);    
private:
    QMap<QString, std::shared_ptr<argExtract>> mArgExtracts;
    QString mCurUuid;
};

//Q_DECLARE_METATYPE(Json::Value)

#endif // ARGUMENTPARSE_H
