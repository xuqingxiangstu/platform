#ifndef ANALYSISRULE_H
#define ANALYSISRULE_H

#include <QString>
#include <QDateTime>

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
private:
    QString mSegmentationMark = "\n";               ///< 分割标志（每帧之间分割标志）
    QString mTimeFormat = "yyyy/M/d h:mm:ss.zz";    ///< 时间解析格式
};

#endif // ANALYSISRULE_H
