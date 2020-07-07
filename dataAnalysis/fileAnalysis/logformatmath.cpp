#include "logformatmath.h"
#include <QDebug>
#include "filter.h"

logFormatMath::logFormatMath(std::shared_ptr<analysisRule> rule, const QString &context)
{
    //TODO:根据匹配规则进行匹配

    QStringList values = context.split("\t");

    //qDebug() << values;
    if(values.size() > 4)
    {
        mContexts[VAR_NAME(timeFilter)] = values[0];
        mContexts[VAR_NAME(systemFilter)] = values[1];
        mContexts[VAR_NAME(msgDirectFilter)] = values[2];
        mContexts[VAR_NAME(frameTypeFilter)] = values[3];
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
