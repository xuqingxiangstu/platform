#include "fileanalysisbusiness.h"

fileAnalysisBusiness::fileAnalysisBusiness()
{

}

void fileAnalysisBusiness::onAnalysis(QVector<QPair<QString, QString> > fileInfo)
{
    //step1：显示进度条窗体

    //step2：判断文件是否存在，如果不存在则显示消息区消息

    //step3：交由文件分析处理(线程池，多线程解析)

    //emit toAnalysis(fileInfo);
}
