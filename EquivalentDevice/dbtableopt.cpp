#include "dbtableopt.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlField>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QDomElement>

DBTableOpt *DBTableOpt::mInstance = nullptr;

DBTableOpt::DBTableOpt()
{
    mDb = QSqlDatabase::addDatabase( "QSQLITE" );

    QString filePath = QDir::currentPath() + "/net_ptl_eq.db";

    QFileInfo info(filePath);

    if(!info.exists(filePath))
    {
        QMessageBox::warning(0, QObject::tr("Database Error"),filePath);
    }
    mDb.setDatabaseName(filePath);

    if(!mDb.open() )
    {
        QMessageBox::warning(0, QObject::tr("Database Error"),mDb.lastError().text());
    }
}
bool DBTableOpt::getValueAllFlowRecord(QString SystemUUID, Json::Value &value)
{
    bool Ok=false;
    QString sql = "SELECT * from flow_record_table WHERE SYSTEM_UUID="+QString("'%1'").arg(SystemUUID);
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        //解析出系统表一行的内容
        for(int index =0; index<rec.count();index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }
            else
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }
        value.append(item);
    }
    return true;
}
bool DBTableOpt::getSystemDev(QString systemUuid,Json::Value &value)
{
    bool result =true;
    QString sql=QString("SELECT * from system_interface_table WHERE SYSTEM_UUID='%1'").arg(systemUuid);
    QSqlQuery query(sql,mDb);
    while(query.next())
    {
       QSqlRecord rec = query.record();
       QString Describe,DevName;
       Json::Value Item;
       Item.clear();
       rec = query.record();
       Item["DEV_TYPE"]=rec.value("DEV_TYPE").toString().toStdString();
       result &= getDevDescribe(rec,Describe,DevName);
       Item["UUID"]=rec.value("DEV_UUID").toString().toStdString();
       Item["DEV_NAME"]=DevName.toStdString();
       Item["DESCRIBE"]=Describe.toStdString();
       value.append(Item);
    }
    return result;
}
bool DBTableOpt::getValueAllSubSystemInfo(Json::Value &value)
{
    bool Ok =false;
    QString sql = "SELECT * from system_table";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        //解析出系统表一行的内容
        for(int index =0; index<rec.count();index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }
            else
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }
        value.append(item);
    }
    return value.isNull();
}
bool DBTableOpt::deleteFlow(QString uuid)
{
    bool result =false;
    QString sql = "DELETE from flow_record_table WHERE UUID="+QString("'%1'").arg(uuid);
    QSqlQuery query(mDb);
    result = query.exec(sql);
    return result;
}
bool DBTableOpt::deleteSubSystem(QString SystemUuid)
{
    bool result=false;
    QString sql = "SELECT * from system_interface_table WHERE SYSTEM_UUID="+QString("'%1'").arg(SystemUuid);
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    if(query.next())
    {
        result=true;
        rec = query.record();
        //2.删除系统对应的所有设备
        //1)UDP设备
        if(!QString::compare(rec.value("DEV_TYPE").toString(),"UDP"))
        {
            sql = "DELETE from udp_table WHERE UUID="+QString("'%1'").arg(rec.value("DEV_UUID").toString());
        } //2)UART设备
        else if(!QString::compare(rec.value("DEV_TYPE").toString(),"UART"))
        {
            sql = "DELETE from uart_table WHERE UUID="+QString("'%1'").arg(rec.value("DEV_UUID").toString());
        }//3) 1553B
        else if(!QString::compare(rec.value("DEV_TYPE").toString(),"1553B"))
        {
            sql = "DELETE from table_1553B WHERE UUID="+QString("'%1'").arg(rec.value("DEV_UUID").toString());
        }else if(!QString::compare(rec.value("DEV_TYPE").toString(),"TCP"))
        {
            sql = "DELETE from tcp_table WHERE UUID="+QString("'%1'").arg(rec.value("DEV_UUID").toString());
        }else if(!QString::compare(rec.value("DEV_TYPE").toString(),"ServerTcp"))
        {
            sql = "DELETE from serverTcp_table WHERE UUID="+QString("'%1'").arg(rec.value("DEV_UUID").toString());
        }
        result &= query.exec(sql);
        //删除相应的测试流程
        sql = "DELETE from flow_record_table WHERE SYSTEM_UUID="+QString("'%1'").arg(SystemUuid);
        result &= query.exec(sql);
    }
    //4.删除该系统表
    sql = "DELETE from system_table WHERE UUID="+QString("'%1'").arg(SystemUuid);
    result &= query.exec(sql);

    sql = "DELETE from system_interface_table WHERE SYSTEM_UUID="+QString("'%1'").arg(SystemUuid);
    result &= query.exec(sql);
    return result;
}
bool DBTableOpt::getDevDescribe(QSqlRecord systemRec,QString &Describe,QString &DevName)
 {
    bool result=false;
     QString sql = "SELECT * from ";
     QString qstTableName=getDevTableName(systemRec.value("DEV_TYPE").toString());
     sql +=qstTableName+" WHERE UUID="+QString("'%1'").arg(systemRec.value("DEV_UUID").toString());
     QSqlQuery query(sql, mDb);
     if(query.next())
     {
         QSqlRecord rec = query.record();
         DevName = rec.value("DEV_NAME").toString();
         if(!QString::compare(systemRec.value("DEV_TYPE").toString(), "UDP"))
         {
           rec = query.record();
           Describe = rec.value("IP_ADDR").toString()+" : "+rec.value("PORT").toString();
         }
         else if(!QString::compare(systemRec.value("DEV_TYPE").toString(), "UART"))
         {
             rec = query.record();
             Describe = rec.value("PORT_NUM").toString();
             Describe += " : "+rec.value("BAUD_RATE").toString();
             Describe += " : "+rec.value("DATA_BIT").toString();
             Describe += " : "+rec.value("STOP_BIT").toString();
             Describe += " : "+rec.value("PARITY_BIT").toString();
         }
         else if(!QString::compare(systemRec.value("DEV_TYPE").toString(), "1553B"))
         {
              Describe = "Card"+rec.value("CARD_NUM").toString();
              Describe += " : "+rec.value("MODE").toString();
              if(!QString::compare(rec.value("MODE").toString(), "RT"))
              {
                  Describe += rec.value("RT_ADDR").toString();
              }
         }else if(!QString::compare(systemRec.value("DEV_TYPE").toString(), "TCP"))
         {
           rec = query.record();
           Describe = rec.value("IP_ADDR").toString()+" : "+rec.value("PORT").toString();
         }else if(!QString::compare(systemRec.value("DEV_TYPE").toString(), "ServerTcp"))
         {
           rec = query.record();
           Describe = rec.value("IP_ADDR").toString()+" : "+rec.value("PORT").toString();
         }
         result=true;
     }
     return result;
 }
bool DBTableOpt::CheckDevTableRepeat(Json::Value &value)
{
    bool res=false;
    QString Tabletype = QString::fromStdString(value["DEV_TYPE"].asString());
    Json::Value DevInfo=value["DEVINFO"];
    QString sql = "";
    QSqlQuery query(mDb);
    if(!QString::compare(Tabletype, "UDP"))
    {
      sql = "SELECT * from udp_table WHERE IP_ADDR="+QString("'%1'").arg(QString::fromStdString(DevInfo["IP_ADDR"].asString()));
      sql += " AND PORT="+QString("'%1'").arg(QString::fromStdString(DevInfo["PORT"].asString()));
    }
    else if(!QString::compare(Tabletype, "UART"))
    {
        sql = "SELECT * from uart_table WHERE PORT_NUM="+QString("'%1'").arg(QString::fromStdString(DevInfo["PORT_NUM"].asString()));
    }
    else if(!QString::compare(Tabletype, "1553B"))
    {
        QString mode = QString::fromStdString(DevInfo["MODE"].asString());
        if(!QString::compare(mode, "BC"))
         {
             sql = "SELECT * from table_1553B WHERE CARD_NUM="+QString("'%1'").arg(QString::fromStdString(DevInfo["CARD_NUM"].asString()));
         }
         else
         {
             sql = "SELECT * from table_1553B WHERE CARD_NUM="+QString("'%1'").arg(QString::fromStdString(DevInfo["CARD_NUM"].asString()));
             sql += " AND RT_ADDR="+QString("'%1'").arg(QString::fromStdString(DevInfo["RT_ADDR"].asString()));
         }
    }else if(!QString::compare(Tabletype, "TCP"))
    {
      sql = "SELECT * from tcp_table WHERE IP_ADDR="+QString("'%1'").arg(QString::fromStdString(DevInfo["IP_ADDR"].asString()));
      sql += " AND PORT="+QString("'%1'").arg(QString::fromStdString(DevInfo["PORT"].asString()));
    }else if(!QString::compare(Tabletype, "ServerTcp"))
    {
      sql = "SELECT * from serverTcp_table WHERE IP_ADDR="+QString("'%1'").arg(QString::fromStdString(DevInfo["IP_ADDR"].asString()));
      sql += " AND PORT="+QString("'%1'").arg(QString::fromStdString(DevInfo["PORT"].asString()));
    }
    query.exec(sql);
    res =query.next();
    return res;
}
QString DBTableOpt::getDevTableName(QString Tabletype)
{
    QString TableName="";
    if(!QString::compare(Tabletype, "UDP"))
    {
        TableName="udp_table";
    }
    else if(!QString::compare(Tabletype, "UART"))
    {
        TableName="uart_table";
    }
    else if(!QString::compare(Tabletype, "1553B"))
    {
        TableName="table_1553B";
    }else if(!QString::compare(Tabletype, "TCP"))
    {
        TableName="tcp_table";
    }else if(!QString::compare(Tabletype, "ServerTcp"))
    {
        TableName="serverTcp_table";
    }
    return TableName;
}
bool DBTableOpt::addSubSystemFlow(Json::Value &value,QString &uuid)
{
    bool result=false;
    QSqlQuery query(mDb);
    result = CreateXmlFile(QString::fromStdString(value["FLOW_NAME"].asString()),QString::fromStdString(value["CREATE_TIME"].asString()));
    if(result)
    {
        uuid = QUuid::createUuid().toString();
        QString DataValue=QString("'%1'").arg(uuid);
        DataValue += QString(",'%1'").arg(QString::fromStdString(value["SYSTEM_UUID"].asString()));
        DataValue += QString(",'%1'").arg(QString::fromStdString(value["FLOW_NAME"].asString()));
        DataValue += QString(",'%1'").arg(QString::fromStdString(value["CREATE_TIME"].asString()));
        DataValue += QString(",'%1'").arg(QString::fromStdString(value["RECENT_OPEN_TIME"].asString()));
        DataValue += QString(",'%1'").arg(QString::fromStdString(value["NOTE"].asString()));
        QString sql = "insert into flow_record_table(UUID, SYSTEM_UUID, FLOW_NAME,CREATE_TIME,RECENT_OPEN_TIME,NOTE) Values("+DataValue+ ")";
        result = query.exec(sql);
    }
    return result;
}
bool DBTableOpt::CreateXmlFile(QString Name,QString CreatTime)
{
    QString fileName=QDir::currentPath()+"/XmlCfgFile/"+Name+"_"+ CreatTime+".xml";
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;
    QTextStream out(&file);
    QDomDocument DomDoc;
    QDomElement dom = DomDoc.createElement("?xml");
    dom.setAttribute("version", "1.0");
    dom.setAttribute("encoding", "UTF-8");
    QDomElement root = DomDoc.createElement("config");
    dom.appendChild(root);
    DomDoc.appendChild(root);
   // QDomElement Flow = DomDoc.createElement("flow");
   // Flow.setAttribute("name","热待发测试");
   // root.appendChild(Flow);
    DomDoc.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();
    return true;
}
bool DBTableOpt::addSystemDev(Json::Value value,QString &Devuuid)
{
    bool result=false;
    //添加该系统的设备
    QString uuid=QUuid::createUuid().toString();
    QString sql="";
    QSqlQuery query(mDb);
    Devuuid=QUuid::createUuid().toString();
    Json::Value Item=value["DEVINFO"];
    if(!CheckDevTableRepeat(value))
    {
        QString qstrDevItem = QString("'%1'").arg(Devuuid);
        qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["DEV_NAME"].asString()));
        if(!QString::compare(QString::fromStdString(value["DEV_TYPE"].asString()), "UDP"))
        {
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["IP_ADDR"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["PORT"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["RESERVE"].asString()));
            sql = "insert into udp_table(UUID,DEV_NAME,IP_ADDR,PORT,RESERVE) Values("+qstrDevItem+")";

        }
        else if(!QString::compare(QString::fromStdString(value["DEV_TYPE"].asString()), "UART"))
        {
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["PORT_NUM"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["BAUD_RATE"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["DATA_BIT"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["STOP_BIT"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["PARITY_BIT"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["RESERVE"].asString()));
            sql = "insert into uart_table(UUID,DEV_NAME,PORT_NUM, BAUD_RATE,DATA_BIT,STOP_BIT,PARITY_BIT,RESERVE) Values("\
                    +qstrDevItem+")";
        }
        else if(!QString::compare(QString::fromStdString(value["DEV_TYPE"].asString()), "1553B"))
        {
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["CARD_NUM"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["MODE"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["RT_ADDR"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["RESERVE"].asString()));
            sql = "insert into table_1553B(UUID,DEV_NAME, CARD_NUM, MODE,RT_ADDR,RESERVE) Values("+qstrDevItem+")";
        }else if(!QString::compare(QString::fromStdString(value["DEV_TYPE"].asString()), "TCP"))
        {
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["IP_ADDR"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["PORT"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["RESERVE"].asString()));
            sql = "insert into tcp_table(UUID,DEV_NAME,IP_ADDR,PORT,RESERVE) Values("+qstrDevItem+")";
        }else if(!QString::compare(QString::fromStdString(value["DEV_TYPE"].asString()), "ServerTcp"))
        {
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["IP_ADDR"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["PORT"].asString()));
            qstrDevItem += QString(",'%1'").arg(QString::fromStdString(Item["RESERVE"].asString()));
            sql = "insert into serverTcp_table(UUID,DEV_NAME,IP_ADDR,PORT,RESERVE) Values("+qstrDevItem+")";
        }
        result = query.exec(sql);
    }
    if(result)
    {
         QString qstrSystem=QString("'%1'").arg(uuid);
         qstrSystem += QString(",'%1'").arg(QString::fromStdString(value["SYSTEM_UUID"].asString()));
         qstrSystem += QString(",'%1'").arg(QString::fromStdString(Item["DEV_NAME"].asString()));
         qstrSystem += QString(",'%1'").arg(QString::fromStdString(value["DEV_TYPE"].asString()));
         qstrSystem += QString(",'%1'").arg(Devuuid);
         sql = "insert into system_interface_table(UUID, SYSTEM_UUID, DEV_NAME, DEV_TYPE,DEV_UUID)  Values("+qstrSystem+")";
         result &= query.exec(sql);
    }
    return result;
}
bool DBTableOpt::alterFlow(QString uuid,QString Name,QString Content)
{
    bool result=false;
    QSqlQuery query(mDb);
    QString sql="UPDATE flow_record_table SET "+Name+"="+QString("'%1'").arg(Content);
    sql += "WHERE UUID=" + QString("'%1'").arg(uuid);
    result = query.exec(sql);
    return result;
}
bool DBTableOpt::alterSubSystem(QString uuid,QString Name)
{
    bool result =false;
    QString sql="select * from system_table WHERE UUID="+QString("'%1'").arg(uuid);
    QSqlQuery query(sql,mDb);
    if(query.next())
    {
        result =true;
        //1.更改系统名
        QString sql="UPDATE system_table SET SYSTEM_NAME="+QString("'%1'").arg(Name);
        sql += " WHERE UUID=" + QString("'%1'").arg(uuid);
        result &= query.exec(sql);
        //2.更改对应的流程名
        sql="UPDATE flow_record_table SET SYSTEM_NAME="+QString("'%1'").arg(Name);
        sql += " WHERE SYSTEM_UUID=" + QString("'%1'").arg(uuid);
        result &= query.exec(sql);
    }
    return result;
}
bool DBTableOpt::addSubSystem(std::vector<QString> &sysArr, QString &sysUuid,QString Note)
{
    bool result=false;
    QSqlQuery query(mDb);
    QString sql="";
    QString uuid=QUuid::createUuid().toString();
    //添加分系统
    sysUuid=uuid;
    QString qstrSystem=QString("'%1'").arg(uuid);
    qstrSystem += QString(",'%1'").arg(sysArr[0]);
    qstrSystem += QString(",'%1'").arg(sysArr[1]);
    qstrSystem += QString(",'%1'").arg(Note);
    sql = "insert into system_table(UUID, SYSTEM_NAME, SYSTEM_TYPE, RESERVE)  Values("+qstrSystem+")";
    result = query.exec(sql);
    return result;
}
bool DBTableOpt::deleteSystemDev(QString uuid)
{
    bool result=false;

    QString sql = "SELECT * from system_interface_table WHERE DEV_UUID="+QString("'%1'").arg(uuid);
    QSqlQuery query(sql,mDb);
    QSqlRecord rec = query.record();
    if(query.next())
    {
        rec = query.record();
        //1.删除系统表的记录
        sql = "DELETE from system_interface_table WHERE DEV_UUID="+QString("'%1'").arg(uuid);
        result = query.exec(sql);
        //删除对应的硬件接口
        QString DevTableName=getDevTableName(rec.value("DEV_TYPE").toString());

        sql = "DELETE from "+QString("%1").arg(DevTableName);
        sql += " WHERE UUID="+QString("'%1'").arg(uuid);
        result &= query.exec(sql);
    }
    return result;
}
bool DBTableOpt::getValuesFrameParam(int tableNum, Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT * from params_table WHERE params_table.TABLE_NUM=";
    sql += QString::number(tableNum, 16);
    sql += " ORDER BY params_table.\"INDEX\" ASC";

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        rec = query.record();
        Json::Value tmpV;
        for(int index = 0; index < rec.count(); index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                tmpV[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }
            else
            {
                tmpV[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }
        value.append(tmpV);
    }

    return value.isNull();
}

bool DBTableOpt::getValueFrameParamSys(Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT SYSTEM FROM params_table GROUP BY SYSTEM";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
        }
        value.append(item);
    }
    return value.isNull();
}

bool DBTableOpt::getValueFrameParamCMD(QString system,Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT CMD_TYPE FROM params_table WHERE SYSTEM="+QString("'%1'").arg(system);
            sql +="GROUP BY CMD_TYPE";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
        }
        value.append(item);
    }
    return value.isNull();
}
bool DBTableOpt::getValueFrameParamGroup(QString cmdType, Json::Value &value)
{
    bool Ok = false;
    QString sql  = "SELECT GROUP_NAME,CMD_TYPE,TABLE_NUM FROM params_table WHERE CMD_TYPE ="+QString("'%1'").arg(cmdType);
            sql += "GROUP BY GROUP_NAME";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
        }
        value.append(item);
    }
    return value.isNull();
}

bool DBTableOpt::getValueFrameParamPar(QString group, Json::Value &value)
{
    bool Ok = false;
    QString sql  = "SELECT * FROM params_table WHERE GROUP_NAME ="+QString("'%1'").arg(group);
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
        }
        value.append(item);
    }
    return value.isNull();
}

bool DBTableOpt::getRecTestNotes(Json::Value &value)
{
    bool Ok = false;
    QString sql  = "SELECT * FROM simulat_table";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    Json::Value sysName;
    Json::Value recordUuid;
    Json::Value flowName;
    while(query.next())
    {
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            if(rec.fieldName(index).toStdString() == "SYSTEM_NAME"){
               sysName.append(rec.value(index).toString().toStdString());
            }else if(rec.fieldName(index).toStdString() == "RECORD_UUID"){
               recordUuid.append(rec.value(index).toString().toStdString());
            }else if(rec.fieldName(index).toStdString() == "FLOW_NAME"){
                flowName.append(rec.value(index).toString().toStdString());
            }
        }
    }
    value["SYSTEM_NAME"] = sysName;
    value["RECORD_UUID"] = recordUuid;
    value["FLOW_NAME"] = flowName;
    return value.isNull();
}

bool DBTableOpt::setRecTestNotes(Json::Value &value)
{
    bool result=false;
    QSqlQuery query(mDb);
    QString delSql = "delete from simulat_table";
    if(query.exec(delSql)){
        for(int i = 0;i<value["SYSTEM_NAME"].size();i++){
            QString sql="";
            QString uuid=QUuid::createUuid().toString();
            //添加分系统
            QString qstrSystem=QString("'%1'").arg(uuid);
            qstrSystem += QString(",'%1'").arg(QString::fromStdString(value["SYSTEM_NAME"][i].asString()));
            qstrSystem += QString(",'%1'").arg(QString::fromStdString(value["RECORD_UUID"][i].asString()));
            qstrSystem += QString(",'%1'").arg(QString::fromStdString(value["FLOW_NAME"][i].asString()));

            sql = "insert into simulat_table(UUID, SYSTEM_NAME,RECORD_UUID,FLOW_NAME)  Values("+qstrSystem+")";
            result &= query.exec(sql);
        }
    }
    return result;
}

bool DBTableOpt::excelToDb(const std::vector<QString> &msgBox)
{
    bool result=false;
    QSqlQuery query(mDb);

    QString sql="";
    QString selectSql  = "select * from params_table where TABLE_NUM = "+QString("'%1'").arg(msgBox[0]);
            selectSql += "AND CODING_NUM = "+QString("'%1'").arg(msgBox[1]);

    if(query.exec(selectSql)){

        QString delSql  = "delete from params_table where TABLE_NUM = "+QString("'%1'").arg(msgBox[0]);
                delSql += "AND CODING_NUM = "+QString("'%1'").arg(msgBox[1]);
        query.exec(delSql);
    }

    QString qstrSystem=QString("'%1'").arg(msgBox[0]);
    for(int i = 1;i<msgBox.size();i++){
        qstrSystem += QString(",'%1'").arg(msgBox[i]);
    }
    sql = "insert into params_table(TABLE_NUM,CODING_NUM,FRAME_TYPE,INFO_WORD_TYPE,IS_ACK,CMD_TYPE,GROUP_NAME,PARAM_NAME,PARAM_BYTE_START_POS,PARAM_BYTE_SIZE,PARAM_BIT_START_POS,PARAM_BIT_SIZE,L_B_ENDIAN,INIT_VALUE,MIN_VALUE,MAX_VALUE,DATA_TYPE,SYSTEM,VALUE_MEAN)  Values("+qstrSystem+")";

    result &= query.exec(sql);
    return result;
}

bool DBTableOpt::dbToExcelTableNum(std::vector<QString> &msgBox)
{
    bool Ok = false;
    QString sql = "SELECT TABLE_NUM FROM params_table GROUP BY TABLE_NUM";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            msgBox.push_back(rec.value(index).toString());
        }
    }
    return Ok;
}

bool DBTableOpt::dbToExcel(QString tableNum,Json::Value &msgBox)
{
    bool Ok = false;
    QString sql = "select * from params_table where TABLE_NUM = "+QString("'%1'").arg(tableNum);

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
        }
        msgBox.append(item);
    }
    return Ok;
}

bool DBTableOpt::deleteTable(QString tableName)
{
    bool Ok = false;
    QString sql = "delete from "+QString("'%1'").arg(tableName);

    QSqlQuery query(mDb);

    if(query.exec(sql)){
        Ok = true;
    }
    return Ok;

}

DBTableOpt::~DBTableOpt()
{

}
