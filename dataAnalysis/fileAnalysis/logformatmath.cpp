#include "logformatmath.h"
#include <QDebug>
#include "filter.h"
#include "../standardtime.h"
#include <exception>
#include <sstream>
#include "../icdAdapter/icdmanager.h"

logFormatMath::logFormatMath(std::shared_ptr<analysisRule> rule, const QString &context)
{ 
    std::ostringstream errInfo;

    //step1:根据日志分割符进行分割
    QString sMark = rule->getLogSegmentationMark();

    QStringList values = context.split(sMark);

    QStringList logF = rule->getLogFormat();

    if(logF.size() != values.size())
    {
        //日志分割后和实际规则不一致，提示用户，不进行解析
        errInfo.str("");
        errInfo << "日志分割后和解析规则不一致(" << sMark.toStdString() << ",";
        for(QString f : logF)
        {
            errInfo << f.toStdString() << "\t";
        }
        errInfo << ")";
        throw std::runtime_error(errInfo.str());
    }
    else
    {
        //逐个匹配
        for(int index = 0; index < logF.size(); index++)
        {
            if((logF.at(index)).compare("消息内容") == 0)
            {
                mVaildMsg = values.at(index);
            }
            else
            {
                QString key = chToEn(logF.at(index));

                if(key.compare(VAR_NAME(timeFilter)) == 0)
                {
                    QDateTime tmpTime = QDateTime::fromString( values.at(index), rule->getTimeFormat());
                    if(!tmpTime.isValid())
                    {
                        //时间格式无效，提示用户
                        errInfo.str("");
                        errInfo << "日期格式设置和实际不一致(" << values.at(index).toStdString() << " , " << rule->getTimeFormat().toStdString() << ")";

                        throw std::runtime_error(errInfo.str());
                    }
                }
                else
                {
                    mFilterItems.append(key);
                }

                mContexts[key] = values.at(index);
            }
        }
    }

}

logFormatMath::~logFormatMath()
{

}

QString logFormatMath::chToEn(const QString &ch)
{
    QString en;

    if(ch.compare("时间") == 0)
    {
        en = VAR_NAME(timeFilter);
    }
    else if(ch.compare("通信系统") == 0)
    {
        en = VAR_NAME(systemFilter);
    }
    else if(ch.compare("通信方向") == 0)
    {
        en = VAR_NAME(msgDirectFilter);
    }
    else if(ch.compare("消息类型") == 0)
    {
        en = VAR_NAME(frameTypeFilter);
    }

    return en;
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
