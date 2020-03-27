#include "dragrole.h"

dragRole::dragRole():
    mNodeProperty(nullptr)
{

}

std::shared_ptr<dragRole> dragRole::clone()
{
    dragRole *obj = new dragRole();

    obj->mCurCmdType = this->mCurCmdType;
    obj->mCurTable = this->mCurTable;
    obj->mCurCoding = this->mCurCoding;

    obj->mNodeProperty = this->mNodeProperty->clone();

    return std::shared_ptr<dragRole>(obj);
}

void dragRole::setTableAndCoding(int table, int coding)
{
    mCurTable = table;
    mCurCoding = coding;

    if(mNodeProperty)
    {
        mNodeProperty->setCodingNum(mCurCoding);
        mNodeProperty->setTableNum(mCurTable);
    }
}

#if 0
dragRole::dragRole(const dragRole &other)
{

}

dragRole::dragRole(const dragRole &&other)
{

}

dragRole &dragRole::operator=(const dragRole &other)
{
    if(this == &other)
        return *this;

    return *this;
}
#endif
