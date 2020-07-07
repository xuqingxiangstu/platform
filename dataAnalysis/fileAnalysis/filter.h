#ifndef FILTER_H
#define FILTER_H

#include <QDateTime>
#include <QMap>
#include "../standardtime.h"

#define VAR_NAME(var) (#var)

/**
 * @brief The filter class  数据过滤
 */

class filter
{
public:    
    /**
     * @brief isMeet    是否满足
     * @param context
     * @return
     */
    virtual bool isMeet(const QString &context);

    /**
     * @brief setFilterCondition    设置过滤条件
     * @param cond                  条件
     */
    virtual void setFilterCondition(const QStringList &cond);
};

/**
 * @brief The timeFilter class  时间过滤
 */

class timeFilter : public filter
{
public:
    bool isMeet(const QString &context) override
    {
        bool res = false;

        QDateTime curTime = standardTime::fromString(context);

        if(!curTime.isValid())
        {
            res = false;
        }
        else
        {
            if( (curTime >= mStartTime) && (curTime <= mStopTime))
            {
                res = true;
            }
        }

        return res;
    }

    void setFilterCondition(const QStringList &times) override
    {
        if(times.size() > 1)
        {
            mStartTime = standardTime::fromString(times.at(0));
            mStopTime = standardTime::fromString(times.at(1));
        }
    }
private:
    QDateTime mStartTime;   ///< 起始时间
    QDateTime mStopTime;    ///< 终止时间
};

/**
 * @brief The systemFilter class    系统过滤
 */
class systemFilter : public filter
{
public:
    bool isMeet(const QString &context) override
    {
        return mSystems.contains(context);
    }

    void setFilterCondition(const QStringList &systems) override {mSystems = systems;}
private:
    QStringList mSystems;   ///< 系统
};

/**
 * @brief The frameTypeFilter class 帧类型过滤
 */
class frameTypeFilter : public filter
{
public:
    bool isMeet(const QString &context) override
    {
        return mFrameType.contains(context);
    }

    void setFilterCondition(const QStringList &frameType) override {mFrameType = frameType;}
private:
    QStringList mFrameType; ///< 帧类型
};

class msgDirectFilter : public filter
{
public:
    bool isMeet(const QString &context) override;

    void setFilterCondition(const QStringList &msgDirect) override {mMsgDirect = msgDirect;}

private:
    QStringList mMsgDirect; ///< 消息方向
};

/**
 * @brief The filterItem class  过滤项目（所有）
 */
class filterItem
{
public:
    /**
     * @brief items 过滤项目
     * @return
     */
    static QStringList items();

    /**
     * @brief chName    中文名称
     * @param item      项目
     * @return
     */
    static QString chName(const QString &item);
};

/**
 * @brief The filterManager class   过滤管理
 */
class filterManager
{
public:
    filterManager();
    ~filterManager();
public:
    /**
     * @brief addFilter 添加过滤
     * @param name      名称
     * @param fl        过滤算法
     */
    void addFilter(const QString &name, filter *fl);

    /**
     * @brief isMeet    是否满足
     * @param name      名称
     * @param context   内容
     * @return
     */
    bool isMeet(const QString &name, const QString &context);
private:
    QMap<QString, filter*> mFilterManager;
};

#endif // FILTER_H
