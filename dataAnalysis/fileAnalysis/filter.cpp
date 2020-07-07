#include "filter.h"

/*************************************/

QStringList filterItem::items()
{
    QStringList item;

    item.append(VAR_NAME(timeFilter));
    item.append(VAR_NAME(systemFilter));
    item.append(VAR_NAME(frameTypeFilter));
    item.append(VAR_NAME(msgDirectFilter));

    return item;
}

QString filterItem::chName(const QString &item)
{
    QString value = "";

    QMap<QString, QString> tmpMap;
    tmpMap[VAR_NAME(timeFilter)] = "时间";
    tmpMap[VAR_NAME(systemFilter)] = "系统";
    tmpMap[VAR_NAME(frameTypeFilter)] = "帧类型";
    tmpMap[VAR_NAME(msgDirectFilter)] = "消息方向";

    if(tmpMap.contains(item))
    {
        value = tmpMap[item];
    }

    return value;
}

/*************************************/

filterManager::filterManager()
{

}

filterManager::~filterManager()
{
    for(auto itor = mFilterManager.begin(); itor != mFilterManager.end(); )
    {
        delete itor.value();
        mFilterManager.erase(itor++);
    }
}

void filterManager::addFilter(const QString &name, filter *fl)
{
    mFilterManager[name] = fl;
}

bool filterManager::isMeet(const QString &name, const QString &context)
{
    bool res = false;

    if(mFilterManager.contains(name))
    {
        res = mFilterManager[name]->isMeet(context);
    }
    else
    {
        //没有过滤则认为有效
        res = true;
    }

    return res;
}
