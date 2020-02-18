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
            int frameLenStartPos = 0, frameLenByteSize = 0;
            int frameLenCalStartPos = 0, frameLenCalToEnd = 0;
            int frameCodeStartPos = 0, frameCodeByteSize = 0;
            int frameInfoTypeStartPos = 0, frameInfoTypeByteSize = 0;
            int frameInfoCntStartPos = 0, frameInfoCntByteSize = 0;
            int frameCntStartPos = 0, frameCntByteSize = 0;
            std::string checkType = "";            
            int frameCheckStartPos = 0, frameCheckByteSize = 0;
            int checkCalStartPos = 0, checkCalToEnd = 0;

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
            frameLenCalStartPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameLenCalToEnd = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCodeStartPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCodeByteSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameInfoTypeStartPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameInfoTypeByteSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameInfoCntStartPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameInfoCntByteSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCntStartPos = value.toInt(&Ok, 10);

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
            frameCheckStartPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            frameCheckByteSize = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            checkCalStartPos = value.toInt(&Ok, 10);

            tmp = workSheet->read(rowIndex++, columnIndex);
            value = tmp.toString();
            checkCalToEnd = value.toInt(&Ok, 10);

            if( !((checkType == "累加和") || (checkType == "CRC") || (checkType == "异或和")))
            {
                strErr.str("");
                strErr << "校验方式异常(CRC/累加和/异或和)：(" << checkType;
                UT_THROW_EXCEPTION(strErr.str());
            }

            mProtocolMsg = std::make_shared<generalStorageType>
                            (frameHeadInitData, frameHeadSize,
                             frameLenStartPos, frameLenByteSize,
                             frameLenCalStartPos, frameLenCalToEnd,
                             frameCodeStartPos, frameCodeByteSize,
                             frameInfoTypeStartPos, frameInfoTypeByteSize,
                             frameInfoCntStartPos, frameInfoCntByteSize,
                             frameCntStartPos, frameCntByteSize,
                             checkType,
                             frameCheckStartPos, frameCheckByteSize,
                             checkCalStartPos, checkCalToEnd
                             );

            return ;
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

