#ifndef BEARGEXTRACT_H
#define BEARGEXTRACT_H


#include "argextract.h"

/**
 * @brief The beArgExtract class    BE参数提取
 */
class beArgExtract : public argExtract
{
    Q_OBJECT
public:
    beArgExtract(QObject *parent = 0);
    ~beArgExtract();
signals:
    /**
     * @brief writeToDb 存入数据库
     * @param value     值
     */
    void writeToDb(QJsonObject value);

    void showResult(QJsonObject value);

    /**
     * @brief showMessage   显示消息提示
     * @param msg           消息
     * @param state         消息状态
     * -true:正常
     * -false:异常
     */
    void showMessage(QString msg, bool state);
public:
    void extract(const QString &uuid, const Json::Value &otherParam, std::shared_ptr<PfIcdWorkBench::frameObj> frameObj, const Json::Value &result) override;
private:
    /**
     * @brief judge     参数判读
     * @param table     表号
     * @param coding    编码
     * @param value     值
     */
    void judge(const unsigned int &table, const unsigned int &coding, const Json::Value &value, const std::string &srcValue);
private:
    Json::Value mOtherParam;
};


#endif // BEARGEXTRACT_H
