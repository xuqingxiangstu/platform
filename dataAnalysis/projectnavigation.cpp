#include "projectnavigation.h"
#include "ui_projectnavigation.h"
#include "./property/templateproperty.h"
#include "./project/projecttemplate.h"
#include "./fileAnalysis/filter.h"
#include <QVariant>
#include <QFile>
#include <QUuid>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>

projectNavigation::projectNavigation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::projectNavigation),
    mCurCloseItem(nullptr),
    mPreHightUuid("")
{
    ui->setupUi(this);

    mUiUuid = QUuid::createUuid().toString();

    mPopMenu = new QMenu(this);
    mPopMenu->addAction(ui->actionAddFile);    
    mPopMenu->addAction(ui->actionDelete);
    mPopMenu->addAction(ui->actionParse);
    mPopMenu->addAction(ui->actionCloseProject);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &projectNavigation::onDoubleClicked);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &projectNavigation::onClicked);

    connect(mPopMenu, &QMenu::triggered, [=](QAction *action){
       //onMenuTrigger(action);
    });

    connect(ui->treeWidget,&QTreeWidget::customContextMenuRequested,[=](const QPoint &point){
        //mRightMousePoint = point;
        QTreeWidgetItem *currentItem = ui->treeWidget->itemAt(point);
        mCurSelectItem = currentItem;
        mCurCloseItem = currentItem;
        if(currentItem)
        {
            recordRole role = currentItem->data(Name_Index, Qt::UserRole).value<recordRole>();
            if(DataFile_Group_Node == role.nodeType)
            {
                ui->actionCloseProject->setEnabled(false);
                ui->actionAddFile->setEnabled(true);

                if(currentItem->childCount() > 0)
                    ui->actionParse->setEnabled(true);
                else
                    ui->actionParse->setEnabled(false);

                ui->actionDelete->setEnabled(false);
            }
            else if(DataFile_Node == role.nodeType)
            {
                ui->actionCloseProject->setEnabled(false);
                ui->actionAddFile->setEnabled(false);
                ui->actionParse->setEnabled(false);
                ui->actionDelete->setEnabled(true);
            }
            else if(Prj_Node == role.nodeType)
            {
                QString text = "关闭工程 \"";
                text += currentItem->text(Name_Index);
                text += "\"";

                ui->actionCloseProject->setText(text);
                ui->actionCloseProject->setEnabled(true);
                ui->actionAddFile->setEnabled(false);

                //TODO:判断是否有文件
                ui->actionParse->setEnabled(true);

                ui->actionDelete->setEnabled(false);
            }
            else
            {
                ui->actionCloseProject->setEnabled(false);
                ui->actionAddFile->setEnabled(false);
                ui->actionParse->setEnabled(false);
                ui->actionDelete->setEnabled(false);
            }

            mPopMenu->exec(QCursor::pos());
        }

    });

    connect(ui->actionAddFile, &QAction::triggered, [=](){
        onAddFiles();
    });

    connect(ui->actionParse, &QAction::triggered, [=](){
       onAnalysis();
    });

    connect(ui->actionDelete, &QAction::triggered, [=](){
       onDeleteFile();
    });

    connect(ui->actionCloseProject, &QAction::triggered, [=]{
        if(mCurCloseItem)
        {
            recordRole role = mCurCloseItem->data(Name_Index, Qt::UserRole).value<recordRole>();
            onCloseProject(role.proUuid);
        }
    });

    mFileAnalysisBusiness = std::make_shared<fileAnalysisBusiness>();
    connect(this, &projectNavigation::analysis, mFileAnalysisBusiness.get(), &fileAnalysisBusiness::onAnalysis);
    connect(mFileAnalysisBusiness.get(), &fileAnalysisBusiness::showMessage, this, &projectNavigation::showMessage);
}

projectNavigation::~projectNavigation()
{
    delete ui;
}

void projectNavigation::onClicked(QTreeWidgetItem *item, int column)
{
    if(item)
    {
        recordRole role = item->data(Name_Index, Qt::UserRole).value<recordRole>();

        if(DataFile_Node == role.nodeType)
        {
            emit toShowProperty(role.nodeUuid, role.mNodeProperty->getJson());
        }
        else
        {
            emit toShowProperty(role.nodeUuid, Json::Value());
        }

        projectHighlight(role.proUuid);
    }
}

QString projectNavigation::nodeUuidConvertProUuid(const QString &nodeUuid)
{
    QString proUuid;

    QTreeWidgetItemIterator Itor(ui->treeWidget);
    while (*Itor)
    {
        recordRole role = (*Itor)->data(0, Qt::UserRole).value<recordRole>();

        if(role.nodeUuid.compare(nodeUuid) == 0)
        {
            proUuid = role.proUuid;
            break;
        }

        ++Itor;
    }

    return proUuid;
}

void projectNavigation::onPropertyValueChange(QString nodeUuid, QString attr, Json::Value val)
{
    //nodeUuid 转为 proUuid

    onProjectModify(nodeUuidConvertProUuid(nodeUuid));

    //找到uuid对应item

    QTreeWidgetItem *item = findItemByNodeUuid(nodeUuid);
    if(item)
    {
        //更新值
        recordRole role = item->data(Name_Index, Qt::UserRole).value<recordRole>();
        role.mNodeProperty->setProperty(attr.toStdString(), val);
    }

    qDebug() << attr << "_" << val.toStyledString().c_str();
}

void projectNavigation::onSaveProject(QString uuid)
{
    QTreeWidgetItem *curItem = nullptr;

    if(uuid.compare("") == 0)   //保存当前
    {
        curItem = ui->treeWidget->currentItem();
    }
    else
    {
        curItem = findItemByProUuid(uuid);
    }

    if(curItem)
    {
        recordRole role = curItem->data(Name_Index, Qt::UserRole).value<recordRole>();

        if(!mIsModify[role.proUuid])
            return ;

        if(mIsModify[role.proUuid])
        {
            mProjcetCfgManager[role.proUuid]->setDataFiles(getDataFilesJson(role.proUuid));

            mProjcetCfgManager[role.proUuid]->save();

            onProjectAlreadySave(role.proUuid);
        }

        if(uuid.compare("") != 0)
           emit saveProjectOver(role.proUuid);
    }
}

QTreeWidgetItem *projectNavigation::findItemByProUuid(QString uuid)
{
    QTreeWidgetItem *item = nullptr;
    QTreeWidgetItemIterator Itor(ui->treeWidget);
    while (*Itor)
    {
        recordRole role = (*Itor)->data(0, Qt::UserRole).value<recordRole>();

        if(Prj_Node == role.nodeType && role.proUuid.compare(uuid) == 0)
        {
            item = *Itor;
            break;
        }

        ++Itor;
    }

    return item;
}

QTreeWidgetItem *projectNavigation::findItemByNodeUuid(QString uuid)
{
    QTreeWidgetItem *item = nullptr;
    QTreeWidgetItemIterator Itor(ui->treeWidget);
    while (*Itor)
    {
        recordRole role = (*Itor)->data(0, Qt::UserRole).value<recordRole>();

        if(role.nodeUuid.compare(uuid) == 0)
        {
            item = *Itor;
            break;
        }

        ++Itor;
    }

    return item;
}

void projectNavigation::onProjectAlreadySave(QString uuid)
{
    if(!mIsModify.contains(uuid))
    {
        mIsModify[uuid] = false;
    }

    if(!mIsModify[uuid])
        return ;

    QTreeWidgetItemIterator Itor(ui->treeWidget);

    while (*Itor)
    {
        recordRole role = (*Itor)->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(Prj_Node == role.nodeType && role.proUuid.compare(uuid) == 0)
        {
            QString srcText = (*Itor)->text(Name_Index);

            //去除标志
            int index = srcText.lastIndexOf(mModifyFlag);
            if(index != -1)
            {
                srcText = srcText.mid(0, index);

                (*Itor)->setText(Name_Index, srcText);
            }
            break;
        }
        ++Itor;
    }

    mIsModify[uuid]  = false;
}

QStringList projectNavigation::getModifyProject()
{
    QStringList modifyUuid;

    foreach(const QString &key, mIsModify.keys())
    {
        if(mIsModify[key])
            modifyUuid.append(key);
    }

    return modifyUuid;
}

bool projectNavigation::isModify(QString uuid)
{
    if(!mIsModify.contains(uuid))
    {
        mIsModify[uuid] = false;
    }

    return mIsModify[uuid];
}

void projectNavigation::onProjectModify(QString uuid)
{
    if(!mIsModify.contains(uuid))
    {
        mIsModify[uuid] = false;
    }

    if(mIsModify[uuid])
        return ;

    QTreeWidgetItemIterator Itor(ui->treeWidget);

    while (*Itor)
    {
        recordRole role = (*Itor)->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(Prj_Node == role.nodeType && role.proUuid.compare(uuid) == 0)
        {
            QString srcText = (*Itor)->text(Name_Index);
            srcText += mModifyFlag;
            (*Itor)->setText(Name_Index, srcText);
            break;
        }
        ++Itor;
    }
    mIsModify[uuid] = true;
}

void projectNavigation::onDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item)
    {
        recordRole role = item->data(Name_Index, Qt::UserRole).value<recordRole>();

        if(Table_Node == role.nodeType)
        {
            emit showTableWidget(role.proUuid, role.proPath);
        }
        else if(S_Img_Node == role.nodeType)
        {
            emit showSingleImgWidget(role.proUuid, role.proPath);
        }
        else if(Time_Img_Node == role.nodeType)
        {
            emit showMultImgWidget(role.proUuid, role.proPath);
        }
        else if(DataFile_Node == role.nodeType)
        {
            emit showFileWidget(role.proUuid, role.nodeUuid, role.proPath, role.filePath);
        }
    }
}

QTreeWidgetItem *projectNavigation::findPrjFromUuid(const QString &uuid)
{
    QTreeWidgetItem *item = nullptr;

    for(int index = 0; index < ui->treeWidget->topLevelItemCount(); index++)
    {
        QTreeWidgetItem *tmpItem = ui->treeWidget->topLevelItem(index);
        recordRole role = tmpItem->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(role.proUuid.compare(uuid) == 0)
        {
            item = tmpItem;
            break;
        }
    }

    return item;
}

QString projectNavigation::getDbPathByUuid(const QString &uuid)
{
    QString path;

    QTreeWidgetItem *prjItem = findPrjFromUuid(uuid);
    if(!prjItem)
    {
        return path;
    }

    recordRole role = prjItem->data(Name_Index, Qt::UserRole).value<recordRole>();

    path = role.proPath;

    path += "/result/";
    path += uuid;
    path += ".db";

    return path;
}

QVector<QPair<QString, QString>> projectNavigation::getDataFilesJson(const QString &uuid)
{
    QVector<QPair<QString, QString>> resultFiles;

    QTreeWidgetItem *prjItem = findPrjFromUuid(uuid);
    if(!prjItem)
    {
        return resultFiles;
    }

    for(int index = 0; index < prjItem->childCount(); index++)
    {
        auto item = prjItem->child(index);
        recordRole role = item->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(DataFile_Group_Node == role.nodeType)
        {

            for(int subIndex = 0; subIndex < item->childCount(); subIndex++)
            {
                auto subItem = item->child(subIndex);
                recordRole subRole = subItem->data(Name_Index, Qt::UserRole).value<recordRole>();
                if(DataFile_Node == subRole.nodeType)
                {                    
                    resultFiles.append(qMakePair(subRole.filePath, subRole.mNodeProperty->getJson().toStyledString().c_str()));
                }
            }
            break;
        }
    }

    return resultFiles;
}

QVector<QPair<QString, QString>> projectNavigation::getDataFilesUuid(const QString &uuid)
{
    QVector<QPair<QString, QString>> resultFiles;

    QTreeWidgetItem *prjItem = findPrjFromUuid(uuid);
    if(!prjItem)
    {
        return resultFiles;
    }

    for(int index = 0; index < prjItem->childCount(); index++)
    {
        auto item = prjItem->child(index);
        recordRole role = item->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(DataFile_Group_Node == role.nodeType)
        {

            for(int subIndex = 0; subIndex < item->childCount(); subIndex++)
            {
                auto subItem = item->child(subIndex);
                recordRole subRole = subItem->data(Name_Index, Qt::UserRole).value<recordRole>();
                if(DataFile_Node == subRole.nodeType)
                {
                    resultFiles.append(qMakePair(subRole.filePath, subRole.nodeUuid));
                }
            }
            break;
        }
    }

    return resultFiles;
}

void projectNavigation::onAnalysis()
{    
    //step1：获取待解析文件

    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();
    if(!curItem)
    {
        QMessageBox::information(this, "提示", "请选择待解析的工程");
        return ;
    }

    //通过uuid找到工程item
    recordRole role = curItem->data(Name_Index, Qt::UserRole).value<recordRole>();

    auto allFiles = getDataFilesUuid(role.proUuid);

    //step2：解析

    QVector<std::tuple<QString, std::shared_ptr<analysisRule>, std::shared_ptr<filterManager>>> fileInfo;

    for(auto info : allFiles)
    {
        //规则
        std::shared_ptr<analysisRule> rule = std::make_shared<analysisRule>();

        QTreeWidgetItem *nodeItem = findItemByNodeUuid(info.second);

        if(nodeItem)
        {
            //获取解析规则
            recordRole nodeRole = nodeItem->data(Name_Index, Qt::UserRole).value<recordRole>();

            //获取行分隔符
            Json::Value splitJs;
            nodeRole.mNodeProperty->getProperty(PROPERTY_DATAFILE_SPLIT, splitJs);
            if(!splitJs.isNull())
            {
                QString sMark = "";

                //modify xqx 2020-7-27 16:20:00 由于属性栏中\r\n显示不出只能为\\r\\n在此转换，去除转移
                std::string tmp = splitJs.asString();

                if(tmp == "\\n")
                {
                    sMark = "\n";
                }
                else if(tmp == "\\r\\n")
                {
                    sMark = "\r\n";
                }

                //end
                rule->setLineSegmentationMark(sMark);
            }

            //获取日期格式
            Json::Value timeFormatJs;
            nodeRole.mNodeProperty->getProperty(PROPERTY_DATAFILE_TIME_FORMAT, timeFormatJs);
            if(!timeFormatJs.isNull())
            {
                rule->setTimeFormat(timeFormatJs.asString().c_str());
            }

            //获取日志分隔符
            Json::Value logSplitJs;
            nodeRole.mNodeProperty->getProperty(PROPERTY_DATAFILE_LOG_SPLIT, logSplitJs);
            if(!logSplitJs.isNull())
            {
                QString sMark = logSplitJs.asString().c_str();

                //TODO:字符串转移为字符 "\\n \n
                if(sMark.compare("\\t") == 0)
                {
                    sMark = "\t";
                }
                else if(sMark.compare("\\n") == 0)
                {
                    sMark = "\n";
                }
                else if(sMark.compare("\\r") == 0)
                {
                    sMark = "\r";
                }
                else if(sMark.compare("\\r\\n") == 0)
                {
                    sMark = "\r\n";
                }
                else if(sMark.compare("\\n\\r") == 0)
                {
                    sMark = "\n\r";
                }

                rule->setLogSegmentationMark(sMark);
            }

            //获取日志格式
            Json::Value logFormatJs;
            nodeRole.mNodeProperty->getProperty(PROPERTY_DATAFILE_LOG_FORMAT, logFormatJs);            
            if(!logFormatJs.isNull() && logFormatJs.isArray())
            {
                QStringList tmpList;
                for(int index = 0; index < logFormatJs.size(); index++)
                {
                    if(logFormatJs[index]["enable"].asBool())
                    {
                        tmpList.append(logFormatJs[index]["name"].asString().c_str());
                    }
                }
                rule->setLogFormat(tmpList);
            }

            //获取过滤算法

            std::shared_ptr<filterManager> flManager = std::make_shared<filterManager>();

            //通信对象
            Json::Value txdxJs;
            nodeRole.mNodeProperty->getProperty(PROPERTY_DATAFILE_TXDX, txdxJs);
            if(!txdxJs.isNull() && txdxJs.isArray())
            {
                systemFilter *ft = new systemFilter();

                QStringList conditions;

                for(int index = 0; index < txdxJs.size(); index++)
                {
                    if(txdxJs[index]["enable"].asBool())
                    {
                        conditions.append(txdxJs[index]["name"].asString().c_str());
                    }
                }

                ft->setFilterCondition(conditions);

                flManager->addFilter(VAR_NAME(systemFilter), ft);
            }

            //通信方向
            Json::Value txfxJs;
            nodeRole.mNodeProperty->getProperty(PROPERTY_DATAFILE_TXFX, txfxJs);
            if(!txfxJs.isNull() && txfxJs.isArray())
            {
                msgDirectFilter *ft = new msgDirectFilter();

                QStringList conditions;

                for(int index = 0; index < txfxJs.size(); index++)
                {
                    if(txfxJs[index]["enable"].asBool())
                    {
                        conditions.append(txfxJs[index]["name"].asString().c_str());
                    }
                }

                ft->setFilterCondition(conditions);

                flManager->addFilter(VAR_NAME(msgDirectFilter), ft);
            }


            //消息类型
            Json::Value msgTypeJs;
            nodeRole.mNodeProperty->getProperty(PROPERTY_DATAFILE_MSG_TYPE, msgTypeJs);
            if(!msgTypeJs.isNull() && msgTypeJs.isArray())
            {
                frameTypeFilter *ft = new frameTypeFilter();

                QStringList conditions;

                for(int index = 0; index < msgTypeJs.size(); index++)
                {
                    if(msgTypeJs[index]["enable"].asBool())
                    {                        
                        //类型映射
                        QString frameName = msgTypeJs[index]["name"].asString().c_str();

                        int type = icdManager::getInstance()->inverseMapping(frameName);

                        if(-1 != type)
                        {
                            conditions.append(QString::number(type, 10));
                        }
                        else
                        {
                            qDebug() << "[ERROR] not found " << frameName;
                        }
                    }
                }

                ft->setFilterCondition(conditions);

                flManager->addFilter(VAR_NAME(frameTypeFilter), ft);
            }

            QStringList timeCondition;

            //起始时间
            Json::Value startTimeJs;
            nodeRole.mNodeProperty->getProperty(PROPERTY_DATAFILE_START_TIME, startTimeJs);

            if(!startTimeJs.isNull())
            {
                timeCondition.append(startTimeJs.asString().c_str());
            }

            //起始时间
            Json::Value stopTimeJs;
            nodeRole.mNodeProperty->getProperty(PROPERTY_DATAFILE_STOP_TIME, stopTimeJs);

            if(!stopTimeJs.isNull())
            {
                timeCondition.append(stopTimeJs.asString().c_str());
            }

            timeFilter *ft = new timeFilter();
            ft->setFilterCondition(timeCondition);

            flManager->addFilter(VAR_NAME(timeFilter), ft);

            fileInfo.append(std::make_tuple(info.first, rule, flManager));
        }
    }

    QString dbPath = getDbPathByUuid(role.proUuid);    

    emit analysis(role.proUuid, dbPath, fileInfo);
}

void projectNavigation::onDeleteFile()
{
    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();
    if(curItem)
    {
        recordRole role = curItem->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(role.nodeType == DataFile_Node)
        {
            //删除属性
            role.mNodeProperty.reset();

            //更新修改标志
            onProjectModify(role.proUuid);

            //删除item
            deleteItem(curItem);

            //更新属性
        }
    }
}

void projectNavigation::deleteItem(QTreeWidgetItem *item)
{
    if(!item)
        return ;

    QTreeWidgetItem *parentItem = item->parent();
    if(parentItem)
    {
        QTreeWidgetItem *del = parentItem->takeChild(parentItem->indexOfChild(item));
        if(del)
            delete del;
    }
    else
    {
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
    }
}

void projectNavigation::onAddFiles()
{
    //QStringList files = QFileDialog::getOpenFileNames(this, tr("文件选择"), tr("./"), tr("所有文件（*.*)"));
    QFileDialog fileDialog(this);
    //设置窗口的标题
    fileDialog.setWindowTitle("数据文件选择");
    fileDialog.setNameFilter("数据文件(*.dat *.clog *.log)"); //设置一个过滤器

    //这个标志用来设置选择的类型，比如默认是单个文件。QFileDialog::ExistingFiles 多个文件,还可以用来选择文件夹QFileDialog::Directory。QFileDialog::ExistingFile 单个文件。注意这个ExistingFile，单词后面多了一个s 表示选择多个文件。要看清楚了。
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    //弹出对话框
    if (fileDialog.exec() == QDialog::Accepted)
    {
        //strPathList  返回值是一个list，如果是单个文件选择的话，只要取出第一个来就行了。
        QStringList srcFiles = fileDialog.selectedFiles();

        recordRole role = mCurSelectItem->data(Name_Index, Qt::UserRole).value<recordRole>();

        //将文件拷贝到工程目录下

        QStringList dstFiles;

        if(mProjcetCfgManager.contains(role.proUuid))
        {
             dstFiles = mProjcetCfgManager[role.proUuid]->copy(srcFiles);

             QVector<QPair<QString, QString>> tmpInfo;

             for(QString file : dstFiles)
             {
                 tmpInfo.append(qMakePair(file, QString("")));
             }

             createFileNode(mCurSelectItem, role.proUuid, role.proPath, tmpInfo, true);

             mProjcetCfgManager[role.proUuid]->setDataFiles(getDataFilesJson(role.proUuid));

             mProjcetCfgManager[role.proUuid]->save();
        }
    }
}

void projectNavigation::loadProject(const QString &proFile)
{
    if(!QFile::exists(proFile))
        return ;

    QString proPath = proFile.left(proFile.lastIndexOf("/"));

    //加载工程配置
    std::shared_ptr<projectCfg> prjCfg = std::make_shared<projectCfg>();
    prjCfg->load(proPath, "");

    QString uuid = prjCfg->getUuid();

    mProjcetCfgManager[uuid] = prjCfg;

    QTreeWidgetItem *prjItem = createPrjNode(uuid, proPath, prjCfg->getPrjName());

    QTreeWidgetItem *fileGroupItem = createFileGroupNode(uuid, proPath);

    createFileNode(fileGroupItem, uuid, proPath, prjCfg->getDataFiles());

    ui->treeWidget->expandItem(fileGroupItem);

    prjItem->addChild(fileGroupItem);

    prjItem->addChild(createTableNode(uuid, proPath));

    prjItem->addChild(createSingleImgNode(uuid, proPath));

    prjItem->addChild(createTimeImgNode(uuid, proPath));

    prjItem->setExpanded(true);

    ui->treeWidget->addTopLevelItem(prjItem);

    ui->treeWidget->expandItem(prjItem);

    ui->treeWidget->expandAll();

    //expandNode(prjItem);
    projectHighlight(uuid);
}

void projectNavigation::onCloseProject(QString uuid)
{
    QTreeWidgetItem *proItem = findItemByProUuid(uuid);
    if(proItem)
    {
        //step1：查看是否有需要保存
        if(isModify(uuid))
        {
            if(QMessageBox::Yes == QMessageBox::warning(this, "提示", "工程有更改，是否保存",QMessageBox::Yes | QMessageBox::No))
            {
                onSaveProject(uuid);
            }
        }

        //step2：删除item
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(proItem));

        //step3：关闭已打开工作区窗体
        emit closeProject(uuid);
    }
}

void projectNavigation::projectHighlight(QString uuid)
{
    if(mPreHightUuid.compare(uuid) == 0)
        return ;

    QTreeWidgetItem *item = findItemByProUuid(uuid);
    if(item)
    {
        QFont font = item->font(Name_Index);
        font.setBold(true);
        item->setFont(Name_Index, font);
    }

    if(mPreHightUuid.compare("") != 0)
    {
        QTreeWidgetItem *item = findItemByProUuid(mPreHightUuid);
        if(item)
        {
            QFont font = item->font(Name_Index);
            font.setBold(false);
            item->setFont(Name_Index, font);
        }
    }

    mPreHightUuid = uuid;
}

QString projectNavigation::createProject(const QString &name, QString proPath, const QStringList &files)
{
    //创建uuid
    QString uuid = QUuid::createUuid().toString();

    proPath += "/";
    proPath += name;

    QTreeWidgetItem *prjItem = createPrjNode(uuid, proPath, name);

    auto *fileGroupItem = createFileGroupNode(uuid, proPath);

    QVector<QPair<QString, QString>> tmpInfo;

    for(QString file : files)
    {
        tmpInfo.append(qMakePair(file, QString("")));
    }

    createFileNode(fileGroupItem, uuid, proPath, tmpInfo, true);

    prjItem->addChild(fileGroupItem);

    prjItem->addChild(createTableNode(uuid, proPath));

    prjItem->addChild(createSingleImgNode(uuid, proPath));

    prjItem->addChild(createTimeImgNode(uuid, proPath));

    prjItem->setExpanded(true);

    ui->treeWidget->addTopLevelItem(prjItem);

    ui->treeWidget->expandItem(prjItem);

    //创建工程模板
    projectTemplate::create(name, proPath, uuid);

    //加载工程配置
    std::shared_ptr<projectCfg> prjCfg = std::make_shared<projectCfg>();
    prjCfg->load(proPath, uuid);
    prjCfg->setPrjName(name);
    prjCfg->save();

    mProjcetCfgManager[uuid] = prjCfg;

    //存储最近使用路径
    QSettings prjSettings("BJTU", "dataAnalysis");
    QStringList recentlyPrj = prjSettings.value("recently_used").toStringList();
    recentlyPrj.insert(0, proPath + "/" + name + ".prj");
    prjSettings.setValue("recently_used", recentlyPrj);

    projectHighlight(uuid);

    return uuid;
}

QTreeWidgetItem *projectNavigation::createPrjNode(const QString &uuid, const QString &proPath, const QString &name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;
    role.nodeType = Prj_Node;
    role.proUuid = uuid;
    role.nodeUuid = uuid;
    role.proPath = proPath;

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);
    item->setExpanded(true);
    item->setText(Name_Index, name);

    return item;
}

QTreeWidgetItem *projectNavigation::createFileGroupNode(const QString &uuid, const QString &proPath)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;

    role.proPath = proPath;
    role.nodeType = DataFile_Group_Node;
    role.proUuid = uuid;
    role.nodeUuid = QUuid::createUuid().toString();
    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);
    item->setExpanded(true);
    item->setText(Name_Index, "数据文件");

    return item;
}

void projectNavigation::createFileNode(QTreeWidgetItem *fatherItem, const QString &uuid, const QString &proPath, const QVector<QPair<QString, QString>> &filesInfo, bool isInit)
{
    for(auto info : filesInfo)
    {
        QString file = info.first;
        QString jsStr = info.second;

        QTreeWidgetItem *subItem = new QTreeWidgetItem();

        QVariant subData;
        recordRole subRole;
        subRole.proUuid = uuid;
        subRole.proPath = proPath;
        subRole.nodeType = DataFile_Node;
        subRole.filePath = file;
        subRole.nodeUuid = QUuid::createUuid().toString();

        QString showFile = file.right(file.size() - file.lastIndexOf("/") - 1);

        if(isInit)
        {
            Json::Value tmpJs = templateProperty::getInstance()->getProperty("dataFile");
            subRole.mNodeProperty = std::make_shared<nodeProperty>(tmpJs);
            subRole.mNodeProperty->setProperty(PROPERTY_DATAFILE_NAME, Json::Value(showFile.toStdString()));
        }
        else
        {
            Json::Value jsValue;
            Json::Reader jsonReader;
            if (!jsonReader.parse(jsStr.toStdString(), jsValue))
                continue;

            subRole.mNodeProperty = std::make_shared<nodeProperty>(jsValue);
        }

        subData.setValue(subRole);

        subItem->setData(Name_Index,Qt::UserRole,subData);

        subItem->setText(Name_Index, showFile);

        subItem->setExpanded(true);

        if(fatherItem)
        {
            fatherItem->addChild(subItem);
        }
        fatherItem->setExpanded(true);
    }
}

void projectNavigation::expandNode(QTreeWidgetItem *item)
{
    if(!item)
        return ;

    if(item->parent())
    {
        item->parent()->setHidden(false);
        item->parent()->setExpanded(true);
        expandNode(item->parent());
    }
}

QTreeWidgetItem *projectNavigation::createTableNode(const QString &uuid, const QString &proPath)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;
    role.proUuid = uuid;
    role.proPath = proPath;
    role.nodeType = Table_Node;
    role.nodeUuid = QUuid::createUuid().toString();

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);
    item->setExpanded(true);
    item->setText(Name_Index, "表格显示");

    return item;
}

QTreeWidgetItem *projectNavigation::createSingleImgNode(const QString &uuid, const QString &proPath)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;
    role.proUuid = uuid;
    role.proPath = proPath;
    role.nodeType = S_Img_Node;
    role.nodeUuid = QUuid::createUuid().toString();

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);
    item->setExpanded(true);
    item->setText(Name_Index, "单图显示");

    return item;
}

QTreeWidgetItem *projectNavigation::createTimeImgNode(const QString &uuid, const QString &proPath)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;
    role.proUuid = uuid;
    role.proPath = proPath;
    role.nodeType = Time_Img_Node;
    role.nodeUuid = QUuid::createUuid().toString();

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);
    item->setExpanded(true);
    item->setText(Name_Index, "全图显示");

    return item;
}
