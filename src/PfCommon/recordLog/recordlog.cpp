#include "recordlog.h"

#include <QCoreApplication>
#include <QDateTime>
#include <sstream>
#include <QDir>

namespace Pf
{
    namespace PfCommon
    {
        RecordLog::RecordLog(bool isThread):
            mFileMaxSize(RECORD_FILE_SIZE),
            mCurFileSizeCnt(0),
            mFile(nullptr),
            mIsThread(false),
            mRecordType(RECORD_NORMAL)
        {
            mIsThread = isThread;

            if(mIsThread)
            {
                moveToThread(this);

                this->start();
                connect(this, &QThread::finished, this, &RecordLog::onFinished);
            }
        }

        RecordLog::~RecordLog()
        {
            if(mIsThread)
            {
                if (isRunning())
                {
                    quit();
                    wait();
                }
            }
            else
            {
                onFinished();
            }
        }

        void RecordLog::onFinished()
        {
            if (mFile != nullptr)
            {
                closeFile();
            }
        }

        //记录数据
        void RecordLog::record(QString uuid, QByteArray Msg)
        {
            if(mUuid.compare(uuid) != 0)
                return ;

            if (mFile == nullptr)
            {
                createFile();

                _record(Msg);
            }
            else
            {
                if (isUpdate())
                {
                    _record(Msg);

                    closeFile();
                }
                else
                {
                    _record(Msg);
                }
            }
        }

        void RecordLog::_record(QByteArray Msg)
        {
            mFile->write(Msg);

            mFile->flush();

            mCurFileSizeCnt += Msg.size();
        }

        bool RecordLog::isUpdate()
        {
            bool bRes = false;

            if (mRecordType == RECORD_NORMAL)
            {
                bRes = false;
            }
            else if (mRecordType == RECORD_FILE_SIZE)
            {
                if (mCurFileSizeCnt > mFileMaxSize)
                {
                    bRes = true;
                }
            }

            return bRes;
        }


        //创建文件
        bool RecordLog::createFile()
        {
            //创建Log文件夹
            std::ostringstream strCode;
            QString strFile;
            strFile = QCoreApplication::applicationDirPath() + "/logRecord";
            QDir Dir(strFile);
            Dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
            if (!Dir.exists())
            {
                Dir.mkdir(strFile);
            }

            //创建Source文件夹

            strFile = strFile + "/" + QString::fromStdString(mSourceName);

            //QDir Dir(strFile);
            Dir.setPath(strFile);

            Dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
            if (!Dir.exists())
            {
                if (!Dir.mkdir(strFile))
                {
                    strCode.str("");
                    strCode << "[ERROR] 创建文件夹[" << strFile.toStdString() << "]失败";
                    throw std::runtime_error(strCode.str());
                }
            }

            //创建日期文件夹
            strFile = strFile + "/" + QString(QDateTime::currentDateTime().toString("yyyy-MM-dd"));

            Dir.setPath(strFile);
            Dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
            if (!Dir.exists())
            {
                if (!Dir.mkdir(strFile))
                {
                    strCode.str("");
                    strCode << "[ERROR] 创建文件夹[" << strFile.toStdString() << "]失败";
                    throw std::runtime_error(strCode.str());
                }
            }

            mCurFilePath = strFile;

            //创建文件
            mCurFileName = QString(QDateTime::currentDateTime().toString("hh.mm.ss.zzz"));

            strFile = strFile + "/" + mCurFileName + ".dat";

            if (mFile == NULL)
            {
                mFile = new QFile(strFile);

                if (!mFile->open(QIODevice::WriteOnly))
                {
                    strCode.str("");
                    strCode << "[ERROR] 打开文件[" << strFile.toStdString() << "]失败";
                    throw std::runtime_error(strCode.str());
                }
            }
            else
            {
                return false;
            }

            return true;
        }
        //关闭文件
        void RecordLog::closeFile()
        {
            QString StopTime = QDateTime::currentDateTime().toString("hh.mm.ss.zzz");

            QString strReName = mCurFilePath + "/" + mCurFileName + "-" + StopTime + ".dat";

            mFile->rename(strReName);

            mFile->close();
            delete mFile;
            mFile = NULL;
            mCurFileSizeCnt = 0;
        }

        void RecordLog::setRecordType(const enRecordType &type)
        {
            mRecordType = type;
        }

        void RecordLog::setSource(const std::string &name)
        {
            mSourceName = name;
        }

        void RecordLog::setMaxSize(const int &size)
        {
            mFileMaxSize = size;
        }
    }
}
