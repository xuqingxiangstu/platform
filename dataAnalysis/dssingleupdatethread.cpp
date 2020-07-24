#include "dssingleupdatethread.h"

#include "../src/PfSql/paramsTable/parseResult/resulttable.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QUuid>

dssingleUpdateThread::dssingleUpdateThread(QString dbPath, QString uuid, QObject *parent) :
    QThread(parent)
{
    mDbPath = dbPath;
    mUuid = uuid;
}

void dssingleUpdateThread::run()
{
    std::shared_ptr<resultTable> resultTb = std::make_shared<resultTable>(mDbPath, mUuid);

    QJsonArray tableGroupNameJs;
    if(resultTb->getTableGroupName(tableGroupNameJs))
    {
        for(int tableIndex = 0; tableIndex < tableGroupNameJs.size(); tableIndex++)
        {
            QString tableName = tableGroupNameJs.at(tableIndex).toObject().value(RESULT_TABLE_TABLE_NUM).toString();

            QString tableUuid = QUuid::createUuid().toString();

            emit updateTableNode(tableUuid, tableName);

            QJsonArray codingGroupJs;
            if(resultTb->getCodingByTableName(tableName, codingGroupJs))
            {
                for(int codingIndex = 0; codingIndex < codingGroupJs.size(); codingIndex++)
                {
                    QString coding = QString::number(codingGroupJs.at(codingIndex).toObject().value(RESULT_TABLE_CODING_NUM).toInt());
                    QString name = codingGroupJs.at(codingIndex).toObject().value(RESULT_TABLE_NAME).toString();

                    emit updateCodingNode(tableUuid, coding, name);
                }
            }
        }
    }
}
