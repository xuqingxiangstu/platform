#include "projectnavigation.h"
#include "ui_projectnavigation.h"
#include "./property/templateproperty.h"
#include <QVariant>
#include <QUuid>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

projectNavigation::projectNavigation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::projectNavigation)
{
    ui->setupUi(this);

    mUiUuid = QUuid::createUuid().toString();

    mPopMenu = new QMenu(this);
    mPopMenu->addAction(ui->actionAddFile);
    mPopMenu->addAction(ui->actionParse);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &recordNavigation::onItemClicked);

    connect(mPopMenu, &QMenu::triggered, [=](QAction *action){
       //onMenuTrigger(action);
    });

    connect(ui->treeWidget,&QTreeWidget::customContextMenuRequested,[=](const QPoint &point){
        //mRightMousePoint = point;
        QTreeWidgetItem *currentItem = ui->treeWidget->itemAt(point);
        mCurSelectItem = currentItem;
        if(currentItem)
        {
            recordRole role = currentItem->data(Name_Index, Qt::UserRole).value<recordRole>();
            if(DataFile_Group_Node == role.nodeType)
            {
                ui->actionAddFile->setEnabled(true);

                if(currentItem->childCount() > 0)
                    ui->actionParse->setEnabled(true);
                else
                    ui->actionParse->setEnabled(false);
            }
            else if(Prj_Node == role.nodeType)
            {
                ui->actionAddFile->setEnabled(false);

                //TODO:判断是否有文件
                ui->actionParse->setEnabled(true);
            }
            else
            {
                ui->actionAddFile->setEnabled(false);
                ui->actionParse->setEnabled(false);
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
}

projectNavigation::~projectNavigation()
{
    delete ui;
}

QTreeWidgetItem *projectNavigation::findPrjFromUuid(const QString &uuid)
{
    QTreeWidgetItem *item = nullptr;

    for(int index = 0; index < ui->treeWidget->topLevelItemCount(); index++)
    {
        QTreeWidgetItem *tmpItem = ui->treeWidget->topLevelItem(index);
        recordRole role = tmpItem->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(role.uuid.compare(uuid) == 0)
        {
            item = tmpItem;
            break;
        }
    }

    return item;
}

void projectNavigation::onAnalysis()
{
    QStringList allFiles;

    //step1：获取待解析文件

    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();
    if(!curItem)
    {
        QMessageBox::information(this, "提示", "请选择待解析的工程");
        return ;
    }

    //通过uuid找到工程item
    recordRole role = curItem->data(Name_Index, Qt::UserRole).value<recordRole>();

    QTreeWidgetItem *prjItem = findPrjFromUuid(role.uuid);
    if(!prjItem)
    {
        QMessageBox::information(this, "提示", "未找到工程item节点");
        return ;
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
                    allFiles.append(subRole.filePath);
                }
            }

            break;
        }
    }

    qDebug() << allFiles;
    //step2；显示进度条

    //step3：解析
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
        QStringList files = fileDialog.selectedFiles();

        recordRole role = mCurSelectItem->data(Name_Index, Qt::UserRole).value<recordRole>();

        createFileNode(mCurSelectItem, role.uuid, files);
    }
}

void projectNavigation::createProject(const QString &name, const QString &uuid, const QStringList &files)
{
    QTreeWidgetItem *prjItem = createPrjNode(uuid, name);

    auto *fileGroupItem = createFileGroupNode(uuid);

    createFileNode(fileGroupItem, uuid, files);

    prjItem->addChild(fileGroupItem);

    prjItem->addChild(createTableNode(uuid));

    prjItem->addChild(createSingleImgNode(uuid));

    prjItem->addChild(createTimeImgNode(uuid));

    prjItem->setExpanded(true);

    ui->treeWidget->addTopLevelItem(prjItem);

    ui->treeWidget->expandItem(prjItem);
}

QTreeWidgetItem *projectNavigation::createPrjNode(const QString &uuid, const QString &name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;
    role.nodeType = Prj_Node;
    role.uuid = uuid;

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);

    item->setText(Name_Index, name);

    return item;
}

QTreeWidgetItem *projectNavigation::createFileGroupNode(const QString &uuid)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;

    role.nodeType = DataFile_Group_Node;
    role.uuid = uuid;

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);

    item->setText(Name_Index, "数据文件");

    return item;
}

void projectNavigation::createFileNode(QTreeWidgetItem *fatherItem, const QString &uuid, const QStringList &files)
{
    for(QString file : files)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem();

        QVariant subData;
        recordRole subRole;
        subRole.uuid = uuid;
        subRole.nodeType = DataFile_Node;
        subRole.filePath = file;
        subData.setValue(subRole);

        subItem->setData(Name_Index,Qt::UserRole,subData);

        QString showFile = file.right(file.size() - file.lastIndexOf("/") - 1);

        subItem->setText(Name_Index, showFile);

        subItem->setExpanded(true);

        if(fatherItem)
        {
            fatherItem->addChild(subItem);
        }
        fatherItem->setExpanded(true);
    }
}

QTreeWidgetItem *projectNavigation::createTableNode(const QString &uuid)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;
    role.uuid = uuid;
    role.nodeType = Table_Node;

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);

    item->setText(Name_Index, "表格显示");

    return item;
}

QTreeWidgetItem *projectNavigation::createSingleImgNode(const QString &uuid)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;
    role.uuid = uuid;
    role.nodeType = S_Img_Node;

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);


    item->setText(Name_Index, "单图显示");

    return item;
}

QTreeWidgetItem *projectNavigation::createTimeImgNode(const QString &uuid)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    recordRole role;
    role.uuid = uuid;
    role.nodeType = Time_Img_Node;

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);

    item->setText(Name_Index, "全图显示");

    return item;
}
