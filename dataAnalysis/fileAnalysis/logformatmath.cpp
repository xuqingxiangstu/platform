#include "logformatmath.h"
#include <QDebug>
#include "filter.h"
#include "../standardtime.h"

logFormatMath::logFormatMath(std::shared_ptr<analysisRule> rule, const QString &context)
{
    //TODO:根据匹配规则进行匹配

    QStringList values = context.split("\t");

    //qDebug() << values;
    if(values.size() > 4)
    {
        int index = 0;

        //根据输入时间格式转换为标准格式
        QString timeValue = values[index++];

        QDateTime tmpTime = QDateTime::fromString(timeValue, rule->getTimeFormat());
        if(tmpTime.isValid())
        {
            //TODO:时间格式无效，提示用户
        }

        mContexts[VAR_NAME(timeFilter)] = standardTime::toString(tmpTime);
        mContexts[VAR_NAME(systemFilter)] = values[index++];
        mContexts[VAR_NAME(msgDirectFilter)] = values[index++];
        mContexts[VAR_NAME(frameTypeFilter)] = values[index++];
        mVaildMsg = values[index++];
    }
}

logFormatMath::~logFormatMath()
{

}

QString logFormatMath::getValue(const QString &name)
{
    QString value = "";
    if(mContexts.contains(name))
    {
        value = mContexts[name];
    }

    return value;
}
