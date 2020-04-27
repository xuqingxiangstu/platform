#include "partpackage.h"

partPackage::partPackage()
{

}

void partPackage::setSrcPackage(const QByteArray &msg)
{
    mSrcMsg = msg;
    mVaildMsg.clear();

    int startPos = 0;
    int stopPos = 0;

    while(1)
    {
        startPos = mSrcMsg.indexOf(mHead);
        if(-1 == startPos)
            break;

        stopPos = mSrcMsg.right(mSrcMsg.size() - mHead.size()).indexOf(mHead);
        if(-1 == stopPos)
        {
            mVaildMsg.append(mSrcMsg.mid(startPos, mSrcMsg.size() - startPos));
            break;
        }
        else
        {
            stopPos += mHead.size();
            mVaildMsg.append(mSrcMsg.mid(startPos, stopPos - startPos));

            mSrcMsg = mSrcMsg.mid(stopPos, mSrcMsg.size() - stopPos);
        }
    }
}

int partPackage::getVaildPkSize()
{
    mVaildMsg.size();
}

QByteArray partPackage::getVaildMsg(int pkIndex)
{
    if( (pkIndex + 1) > mVaildMsg.size())
        return "";

    return mVaildMsg.at(pkIndex);
}
