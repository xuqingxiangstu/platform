#include "copypaste.h"

copyPaste::copyPaste(QObject *parent):
    QThread(parent),
    mIsFirst(true),
    mCopyItem(nullptr)
{

}

copyPaste::~copyPaste()
{

}

void copyPaste::setCopy(const QTreeWidgetItem *&copyItem)
{
    if(mIsFirst)
        mIsFirst = false;
}

void copyPaste::run()
{

}

bool copyPaste::isPaste()
{
    return !mIsFirst;
}

void copyPaste::startPaste()
{
    if(mIsFirst || !mCopyItem)
        return ;

    std::shared_ptr<dragRole> drapData = mCopyItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

    if( (dragRole::Node_Flow == drapData->getNodeType()))
    {
        //启动线程
    }
    else if(dragRole::Node_Cmd == drapData->getNodeType())
    {

    }
}
