#include "analysis.h"

#include <QFile>
#include <QDebug>

#include "logFormatmath.h"

analysis::analysis(QObject *parent) : QObject(parent)
{

}

void analysis::onAnalysis(QString uuid, QString filePath, std::shared_ptr<analysisRule> rule, std::shared_ptr<filterManager> filterManager)
{
    mCurFile = filePath;
    mCurUuid = uuid;

    QFile fileObj(filePath);
    if(!fileObj.open(QIODevice::ReadOnly))
    {
        QString errInfo = "[ERROR][analysis] open file faild => " + filePath;

        emit showMessage(errInfo, false);

        emit analysisOver();

        return ;
    }

    QByteArray allData = fileObj.readAll();

    //step1：分割帧
    QString tmpAllData = allData;

    QStringList frames = tmpAllData.split(rule->getSegmentationMark());

    int rowIndex = 1;

    foreach (QString frame, frames)
    {
        //根据日志格式，匹配时间、系统、方向、类型、有效数据
        logFormatMath match(rule, frame);

        bool filterRes = true;
        bool res = false;

        //获取待过滤项
        QStringList items = filterItem::items();

        //根据匹配值进行过滤判断
        for(QString item : items)
        {
            QString value = match.getValue(item);
            if(value.compare("") == 0)
            {
                QString errInfo;
                errInfo += "[ERROR]";
                errInfo += "第";
                errInfo += QString::number(rowIndex, 10);
                errInfo += "行->";

                errInfo += filterItem::chName(item);

                errInfo += "获取失败";

                emit showMessage(errInfo, false);

                filterRes = false;
                break;
            }
            else
            {
                res = filterManager->isMeet(item, value);

                filterRes &= res;
            }
        }

        if(filterRes)
        {
            //TODO:解析
        }

        rowIndex++;
    }
}
