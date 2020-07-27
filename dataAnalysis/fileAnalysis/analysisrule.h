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
     * @brief getLineSegmentationMark   获取行分割标志
     * @return
     */
    QString getLineSegmentationMark(){return mLineSegmentationMark;}

    /**
     * @brief setLineSegmentationMark   设置行分割标志
     * @param mark
     * @return
     */
    void setLineSegmentationMark(const QString &mark){mLineSegmentationMark = mark;}

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
    void setLogFormat(const QStringList &format){mLogFormat = format;}

    /**
     * @brief getLogFormat 获取日志格式
     * @return
     */
    QStringList getLogFormat(){return mLogFormat;}

    /**
     * @brief getLogSegmentationMark   获取日志分割标志
     * @return
     */
    QString getLogSegmentationMark(){return mLogSegmentationMark;}

    /**
     * @brief setLogSegmentationMark   设置日志分割标志
     * @param mark
     * @return
     */
    void setLogSegmentationMark(const QString &mark){mLogSegmentationMark = mark;}
private:
    QString mLogSegmentationMark = "\t";                ///< 日志分割标志（每行直接分割）
    QString mLineSegmentationMark = "\n";               ///< 行分割标志（每帧之间分割标志）
    QString mTimeFormat = "yyyy/M/d h:mm:ss.zz";        ///< 时间解析格式
    QStringList mLogFormat;                             ///< 行日志格式
};

Q_DECLARE_METATYPE(std::shared_ptr<analysisRule>)

#endif // ANALYSISRULE_H
