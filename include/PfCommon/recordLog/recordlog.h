#ifndef RECORDLOG_H
#define RECORDLOG_H

/**
 * @file recordLog.h
 * @brief 记录日志，可选择线程/非线程存储，采用Qt方式
 * @author xqx
 * @date 2020-01-07
 * @version V1.0
 *
 */

#include "recordlog_global.h"

#include <QThread>
#include <QFile>
#include <string>
#include <memory>

/** 默认存储大小 50M **/
#define DEFAULT_RECORD_SIZE (50 * 1024 *1024)

namespace Pf
{
    namespace PfCommon
    {
        /**
         * @brief The RecordLog class
         */
        class RECORDLOGSHARED_EXPORT RecordLog : public QThread
        {
            Q_OBJECT
        public:
            /** 文件存储类型 **/
            enum enRecordType{
                RECORD_FILE_SIZE,	///< 按照文件大小存储
                RECORD_NORMAL		///< 按照一个文件存储
            };
        public:
            /**
             * @brief RecordLog 构造函数
             * @param[in] isThread 是否线程存储
             * - true：线程存储
             * - false：非线程存储
             */
            RecordLog(bool isThread = false);
            ~RecordLog();
        public:
            /**
             * @brief setSource 设置数据源名称
             * @param[in] name      名称
             */
            void setSource(const std::string &name);

            /**
             * @brief setRecordType 设置存储类型，单个文件/按大小存储
             * @param[in] type
             * - RECORD_NORMAL：单个文件
             * - RECORD_FILE_SIZE：多个文件(按设置文件大小存储)
             */
            void setRecordType(const enRecordType &type);

            /**
             * @brief setMaxSize    当存储类型为RECORD_FILE_SIZE时，设置存储大小，默认50M
             * @param[in] size          文件大小
             */
            void setMaxSize(const int &size);
        public slots:
            /**
             * @brief record 记录日志操函数
             * @note
             * - 直接调用为单线程
             * - 信号槽形式根据设置是否线程而定
             * @param[in] msg 数据
             */
            void record(QByteArray msg);
        private slots:
            /**
             * @brief onFinished 存储结束
             */
            void onFinished();
        private:
            /**
             * @brief createFile    创建文件
             * @return 是否成功
             * - true：成功
             * - false：失败
             *
             */
            bool createFile();

            /**
             * @brief closeFile
             * 关闭文件
             */
            void closeFile();

            /**
             * @brief _record   记录文件
             * @param msg 内容
             */
            void _record(QByteArray msg);

            /**
             * @brief isUpdate  是否更新
             * @return
             * - true：更新
             * - false：不更新
             */
            bool isUpdate();
        private:
            int mFileMaxSize; ///< 文件最大存储长度
            std::string mSourceName; ///< 文件源名称
            QFile *mFile; ///< 文件句柄
            int mCurFileSizeCnt; ///< 当前记录个数
            QString mCurFileName; ///< 当前文件名称
            QString mCurFilePath; ///< 当前文件路径
            bool mIsThread; ///< 是否线程
            enRecordType mRecordType; ///< 存储方式
        };
    }
}
#endif // RECORDLOG_H
