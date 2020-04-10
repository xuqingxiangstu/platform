#include "protocolconfigure.h"

#include "../../PfCommon/tools/ut_error.h"

#include "../common/type.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        protocolConfigure::protocolConfigure()
        {

        }
        void protocolConfigure::init(QXlsx::Worksheet *workSheet)
        {
            std::ostringstream strErr;
            QVariant tmp;
            QString value;
            bool Ok;
            int frameHeadInitData = 0, frameHeadSize = 0;
            int frameLenStartPos = 0, frameLenByteSize = 0, frameLenBitStart = 0, frameLenBitSize = 0;
            int frameCodeStartPos = 0, frameCodeByteSize = 0, frameCodeBitPos = 0, frameCodeBitSize = 0;
            int frameCntToEndPos = 0, frameCntByteSize = 0;
            std::string checkType = "";
            int frameCheckToEndtPos = 0, frameCheckByteSize = 0;
            int checkCalStartPos = 0, checkCalToEnd = 0;
            std::string isContinue = "";
            int frameCntDeal = 0;

            if(workSheet == nullptr)
            {
                strErr << "worksheet == nullptr";
                UT_THROW_EXCEPTION(strErr.str());
            }

            int i32Rows = workSheet->dimension().lastRow();
            int i32Columns = workSheet->dimension().lastColumn();

            if ((i32Rows < 18) || (i32Columns < 2))
            {
                std::string sheetName = "";
    #ifdef Utf8_Coding
                sheetName = workSheet->sheetName().toUtf8().data();
    #else
                sheetName = workSheet->sheetName().toLocal8Bit().data();
    #endif
                strErr.str("");
                strErr << "[ERROR]" << sheetName << " 行列不合法应该为(18,2), rows=" << std::dec << i32Rows
                    << "columns=" << i32Columns;

                UT_THROW_EXCEPTION(strErr.str());
            }

            int rowIndex = 2;
            int columnIndex = 2;

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            if (value.contains("0x"))
            {
                int size = (value.size() - 2) / 2;
                if ((value.size() - 2) % 2)
                {
                    size++;
                }
                frameHeadSize = size;
                frameHeadInitData = value.toUInt(&Ok, 16);
            }
            else
            {
                //
                int i32Data[8] = { 0 };
                bool ok = false;
                value = value.trimmed();
                QStringList splitResult = value.split(" ");

                int i32Cnt = splitResult.size();

                for(int i = 0; i < i32Cnt; i++)
                {
                    i32Data[i] = splitResult.at(i).toInt(&ok, 16);
                }

                frameHeadSize = i32Cnt;

                unsigned long long u64TmpData = 0;
                for (int i = 0; i < i32Cnt; i++)
                {
                    u64TmpData |= (i32Data[i] << (i32Cnt - i - 1) * 8);
                }
                frameHeadInitData = u64TmpData;
            }

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameLenStartPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameLenByteSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameLenBitStart = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameLenBitSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCodeStartPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCodeByteSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCodeBitPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCodeBitSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCntToEndPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCntByteSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
    #ifdef Utf8_Coding
            checkType = value.toUtf8().data();
    #else
            checkType = value.toLocal8Bit().data();
    #endif

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCheckToEndtPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCheckByteSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            checkCalStartPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            checkCalToEnd = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
    #ifdef Utf8_Coding
            isContinue = value.toUtf8().data();
    #else
            isContinue = value.toLocal8Bit().data();
    #endif

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCntDeal = value.toInt(&Ok, 10);

            checkParamValid(
                        frameHeadInitData, frameHeadSize,
                                         frameLenStartPos, frameLenByteSize, frameLenBitStart, frameLenBitSize,
                                         frameCodeStartPos, frameCodeByteSize, frameCodeBitPos, frameCodeBitSize,
                                         frameCntToEndPos, frameCntByteSize,
                                         checkType,
                                         frameCheckToEndtPos, frameCheckByteSize, checkCalStartPos, checkCalToEnd,
                                         isContinue,
                                         frameCntDeal
                        );

            mProtocolMsg = std::make_shared<variableStorageType>
                            (frameHeadInitData, frameHeadSize,
                             frameLenStartPos, frameLenByteSize, frameLenBitStart, frameLenBitSize,
                             frameCodeStartPos, frameCodeByteSize, frameCodeBitPos, frameCodeBitSize,
                             frameCntToEndPos, frameCntByteSize,
                             checkType,
                             frameCheckToEndtPos, frameCheckByteSize, checkCalStartPos, checkCalToEnd,
                             isContinue,
                             frameCntDeal);

            auto tmp03 = mProtocolMsg->getMessage<variableStorageIndex::variable_head_init_index>();
            return ;
        }

        void protocolConfigure::checkParamValid(unsigned int frameHeadInitData, int frameHeadSize,
                                                        int frameLenStartPos, int frameLenByteSize, int frameLenBitStart, int frameLenBitSize,
                                                        int frameCodeStartPos, int frameCodeByteSize, int frameCodeBitPos, int frameCodeBitSize,
                                                        int frameCntToEndPos, int frameCntByteSize,
                                                        std::string checkType,
                                                        int frameCheckToEndtPos, int frameCheckByteSize, int checkCalStartPos, int checkCalToEnd,
                                                        std::string isContinue,
                                                        int frameCntDeal)
        {
            std::ostringstream strErr;

            if(frameHeadSize < 1)
            {
                strErr.str("");
                strErr << "帧识别码长度索引异常(" << std::dec << frameHeadSize;
                UT_THROW_EXCEPTION(strErr.str());
            }

            if( (frameLenStartPos < 0) || (frameLenByteSize <= 0) || (frameLenBitStart < 0) || (frameLenBitSize < 0) )
            {
                strErr.str("");
                strErr << "帧长度异常(" << std::dec << frameLenStartPos << "," << frameLenByteSize << "," << frameLenBitStart << "," << frameLenBitSize << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            if(frameLenBitSize > 64)
            {
                strErr.str("");
                strErr << "帧长度位大小异常 0< size <64(" << std::dec << frameLenBitSize <<")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            if( (frameCodeStartPos < 0) || (frameCodeByteSize <= 0) || (frameCodeBitPos < 0) || (frameCodeBitSize < 0) )
            {
                strErr.str("");
                strErr << "帧识别码索引异常(" << std::dec << frameCodeStartPos << "," << frameCodeByteSize << "," << frameCodeBitPos << "," << frameCodeBitSize << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            if(frameCodeBitSize > 64)
            {
                strErr.str("");
                strErr << "帧识别码位大小异常 0< size <64(" << std::dec << frameCodeBitSize <<")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            if( (frameCntToEndPos < 0) || (frameCntByteSize <= 0) )
            {
                strErr.str("");
                strErr << "帧计数计算索引异常(" << std::dec << frameCntToEndPos << "," << frameCntByteSize << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            if( (frameCheckToEndtPos < 0) || (frameCheckByteSize <= 0) )
            {
                strErr.str("");
                strErr << "帧校验索引异常(" << std::dec << frameCheckToEndtPos << "," << frameCheckByteSize << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            if( (checkCalStartPos < 0) || (checkCalToEnd < 0) )
            {
                strErr.str("");
                strErr << "帧校验计算索引异常(" << std::dec << checkCalStartPos << "," << checkCalToEnd << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }
        }

        /**
         * @return virtual framePtlCfg *
         */
        std::shared_ptr<protocolConfigure> protocolConfigure::clone()
        {
            protocolConfigure *fix = new protocolConfigure();

            fix->mProtocolMsg = this->mProtocolMsg->clone();

            std::shared_ptr<protocolConfigure> tmp(fix);

            return tmp;
        }

    }
}

