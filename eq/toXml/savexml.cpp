#include "savexml.h"

#include "createsubflow.h"
#include "../dragTree/dragrole.h"
#include "../recordnavigation.h"

saveXml::saveXml(QObject *parent):
    QThread(parent),
    mCurPath(""),
    mCurTree(nullptr)
{

}

saveXml::~saveXml()
{
    if(isRunning())
    {
        quit();

        wait();
    }
}

void saveXml::setSystemInfo(QTreeWidgetItem *item)
{
    mCurSystemItem = item;
}

void saveXml::setTreeWidget(QTreeWidget *widget)
{
    mCurTree = widget;
}

void saveXml::startTask()
{
    if(!isRunning())
        start();
}

void saveXml::run()
{
    if(!mCurTree)
    {
        emit saveOver();
        return ;
    }

    bool isSave = false;
    int processIndex = 0;

    TiXmlElement *rootElement = new TiXmlElement("config");

    //等效信息
    rootElement->LinkEndChild(createEquivalent::element(mCurSystemItem->data(0, Qt::UserRole).value<recordRole>().mNodeProperty.get()));

    //step1:遍历流程节点
    for(int flowIndex = 0; flowIndex < mCurTree->topLevelItemCount(); flowIndex++)
    {
        bool isRootLink = false;

        QTreeWidgetItem *flowItem = mCurTree->topLevelItem(flowIndex);

        //创建xml元素
        TiXmlElement *flowElement = createFlow::element(flowItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>()->getProperty());

        emit saveProgress(processIndex++);

        //step3：遍历指令或参数节点
        for(int cmdOrParamIndex = 0; cmdOrParamIndex < flowItem->childCount(); cmdOrParamIndex++)
        {
            QTreeWidgetItem *cmdOrParamItem = flowItem->child(cmdOrParamIndex);

            TiXmlElement *testItemElement = nullptr;

            //判断是指令还是参数
            dragRole *cmdOrParamRole = cmdOrParamItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();
            auto type = cmdOrParamRole->getNodeType();
            if(dragRole::Node_Cmd == type)  //指令
            {
                testItemElement = createSubFlow::element(mCurSystemItem->data(0, Qt::UserRole).value<recordRole>().mNodeProperty.get(), cmdOrParamRole->getProperty());

                emit saveProgress(processIndex++);
            }
            else if(dragRole::Node_Param_Group == type)//参数
            {
                std::vector<nodeProperty*> subRoles;
                //获取子信息
                for(int index = 0; index < cmdOrParamItem->childCount(); index++)
                {
                    dragRole *tmpRole = (cmdOrParamItem->child(index))->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();
                    subRoles.push_back(tmpRole->getProperty());
                }
                testItemElement = createSubFlow::element(mCurSystemItem->data(0, Qt::UserRole).value<recordRole>().mNodeProperty.get(), cmdOrParamRole->getProperty(), subRoles);

                emit saveProgress(processIndex++);
            }

            if(testItemElement)
            {
                flowElement->LinkEndChild(testItemElement);
                isRootLink = true;
            }
        }

        if(isRootLink)
        {
            rootElement->LinkEndChild(flowElement);
            isSave = true;
        }
    }

    //保存文件
    TiXmlDocument *myDocument = new TiXmlDocument();

    myDocument->LinkEndChild(rootElement);

    if(isSave)
        myDocument->SaveFile(mCurPath.toStdString().c_str());

    emit saveOver();
}


