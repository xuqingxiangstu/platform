#ifndef LOGFORMATMATH_H
#define LOGFORMATMATH_H

#include "analysisrule.h"

#include <memory>
#include <QMap>

/**
 * @brief The logFormatMath class  日志格式匹配（匹配测试时间、系统、消息方向、帧类型、有效数据）
 */

class logFormatMath
{
public:
    /**
     * @brief logFormatMath 日志格式匹配
     * @param rule          规则
     * @param context       原始内容
     */
    logFormatMath(std::shared_ptr<analysisRule> rule, const QString &context);
    ~logFormatMath();
public:
    /**
     * @brief getValue  根据名称获取值
     * @param name
     * @return
     */
    QString getValue(const QString &name);

    /**
     * @brief getVaildMsg   获取有效消息
     * @return
     */
    QString getVaildMsg(){return mVaildMsg;}
private:
    QMap<QString, QString> mContexts;
    QString mVaildMsg;      ///< 有效消息
};

#endif // FORMATMATH_H
