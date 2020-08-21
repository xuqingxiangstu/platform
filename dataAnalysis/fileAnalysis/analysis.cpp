#include "analysis.h"

#include <QFile>
#include <QDebug>

#include "logFormatmath.h"

analysis::analysis(QObject *parent) : QObject(parent)
{
    mArgParse = std::make_shared<argumentParse>();

    connect(mArgParse.get(), &argumentParse::toDataBase, this, &analysis::toDataBase);
    connect(this, &analysis::toParse, mArgParse.get(), &argumentParse::parse);

    connect(mArgParse.get(), &argumentParse::showMessage, this, &analysis::showMessage);
}

void analysis::onAnalysis(QString uuid, QString filePath, std::shared_ptr<analysisRule> rule, std::shared_ptr<filterManager> filterManager)
{
    mCurFile = filePath;
    mCurUuid = uuid;
    QString showInfo;
    QString fileName = filePath.right(filePath.size() - filePath.lastIndexOf("/") - 1);

    QFile fileObj(filePath);
    if(!fileObj.open(QIODevice::ReadOnly))
    {
        QString errInfo = "[ERROR][analysis] open file faild => " + filePath;

        emit showMessage(errInfo, false);

        emit analysisOver(uuid);

        return ;
    }

    QByteArray allData = fileObj.readAll();

    //step1：分割帧
    QString tmpAllData = allData;

    QString sMark = rule->getLineSegmentationMark();

    QStringList frames = tmpAllData.split(sMark, QString::SkipEmptyParts, Qt::CaseSensitive);

    //TODO:分割失败，提示用户
//    if(frames.)

    int rowIndex = 1;

    showInfo = "";
    showInfo += "正在解析 ";
    showInfo += fileName;
    showInfo += " 文件...";
    emit showMessage(showInfo);

    try
    {
        foreach (QString frame, frames)
        {
            if(frame.compare("") == 0)
                continue;

            //根据日志格式，匹配时间、系统、方向、类型、有效数据
            logFormatMath match(rule, frame);

            bool filterRes = true;
            bool res = false;

            //获取待过滤项
            QStringList items = match.getFilterItem();

            if(filterManager != nullptr)    ///无效则认为不进行过滤
            {
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
            }

            if(filterRes)
            {
                //获取帧类型
                QString typeStr = match.getValue(VAR_NAME(frameTypeFilter));
                int type = typeStr.toInt();

                //获取有效数据
                QByteArray msg = match.getVaildMsg().toUtf8();

                Json::Value param;
                param["frameType"] = type;
                param["rowIndex"] = rowIndex;
                param["time"] = match.getValue(VAR_NAME(timeFilter)).toStdString();                                

                //解析
                emit toParse(mCurUuid, param, QByteArray::fromHex(msg));
            }

            emit step(rowIndex);

            rowIndex++;
        }
    }
    catch(std::runtime_error err)
    {
        QString errInfo;
        errInfo += "[ERROR]";
        errInfo += fileName;
        errInfo += " 第 ";
        errInfo += QString::number(rowIndex++, 10);
        errInfo += " 行, ";
        errInfo += QString(err.what());

        emit showMessage(errInfo, false);
    }

    emit analysisOver(uuid);

    showInfo = "";
    showInfo += "解析完成";
    emit showMessage(showInfo);
}
