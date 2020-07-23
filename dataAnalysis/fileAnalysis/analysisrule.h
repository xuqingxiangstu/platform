#ifndef ANALYSISRULE_H
#define ANALYSISRULE_H

#include <QString>
#include <QDateTime>
#include <QObject>
#include <memory>

/**
 * @brief The analysisRule class    解析规则
 */

class analysisRule
{   
public:
    /**
     * @brief getSegmentationMark   获取分割标志
     * @return
     */
    QString getSegmentationMark(){return mSegmentationMark;}

    /**
     * @brief setSegmentationMark   设置分割标志
     * @param mark
     * @return
     */
    void setSegmentationMark(const QString &mark){mSegmentationMark = mark;}

    /**
     * @brief setTimeFormat 设置时间解析格式
     * @param format
     */
    void setTimeFormat(const QString &format){mTimeFormat = format;}

    /**
     * @brief getTimeFormat 获取时间解析格式
     * @return
     */
    QString getTimeFormat(){return mTimeFormat;}

    /**
     * @brief setLogFormat 设置日志格式
     * @param format
     */
    void setLogFormat(const QString &format){mLogFormat = format;}

    /**
     * @brief getLogFormat 获取日志格式
     * @return
     */
    QString getLogFormat(){return mLogFormat;}
private:
    QString mSegmentationMark = "\n";               ///< 分割标志（每帧之间分割标志）
    QString mTimeFormat = "yyyy/M/d h:mm:ss.zz";    ///< 时间解析格式
    QString mLogFormat = "";    ///< 行日志格式
};

Q_DECLARE_METATYPE(std::shared_ptr<analysisRule>)

#endif // ANALYSISRULE_H
