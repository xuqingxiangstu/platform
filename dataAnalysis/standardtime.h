#ifndef STANDARDTIME_H
#define STANDARDTIME_H

#include <memory>
#include <QString>
#include <QDateTime>

/**
 * @brief The standardTime class    标准时间转换
 */

class standardTime
{
public:
    /**
     * @brief fromString    字符串->时间
     * @param time
     * @return
     */
    static QDateTime fromString(const QString &time);

    /**
     * @brief toString  时间->字符串
     * @param time
     * @return
     */
    static QString toString(const QDateTime &time);
private:
    static QString mDateTimeFormat;  ///< 时间格式
};

#endif // STANDARDTIME_H
