#include "subprotocolconfigure.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../common/subProtocol.h"
#include "../common/type.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        subProtocolConfigure::subProtocolConfigure()
        {

        }

        subProtocolConfigure::~subProtocolConfigure()
        {

        }

        std::shared_ptr<subProtocolConfigure> subProtocolConfigure::clone()
        {
            subProtocolConfigure *tmpCfg = new subProtocolConfigure();
            for(auto itor = this->mParamsInfo.begin(); itor != this->mParamsInfo.end(); itor++)
            {
                tmpCfg->mParamsInfo.insert(decltype(tmpCfg->mParamsInfo)::value_type(itor->first, itor->second));
            }

            return std::shared_ptr<subProtocolConfigure>(tmpCfg);
        }

        void subProtocolConfigure::init(QXlsx::Workbook *workBook)
        {
            std::ostringstream strErr;
            QVariant tmp;
            QString value;
            bool Ok;

            if(workBook == nullptr)
            {
                strErr.str("");
                strErr << "workBook == nullptr";

                UT_THROW_EXCEPTION(strErr.str());
            }

            if(workBook->sheetCount() == 0)
            {
                strErr.str("");
                strErr << "sheet == 0";

                UT_THROW_EXCEPTION(strErr.str());
            }

            if(workBook->sheetCount() == 1)
            {
                strErr.str("");
                strErr << "sheet == 1, no sub frame sheet";

                UT_THROW_EXCEPTION(strErr.str());
            }

            for(int index = 1; index < workBook->sheetCount(); index++)
            {
                std::string pId = "", pName = "";
                int byteStart = 0, byteSize = 0;
                int bitStart = 0, bitSize = 0;
                std::string bigSmal;
                int srcMin = 0, srcMax = 0;
                double meanMin = 0, meanMax = 0;
                std::string pCategory = "";
                double d64A = 0, d64B = 0, d64Lsb = 0;
                std::string curSheetName = "";
                unsigned int initData = 0;

                QXlsx::Worksheet *workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(index));

    #ifdef Utf8_Coding
                curSheetName = workSheet->sheetName().toUtf8().data();
    #else
                curSheetName = workSheet->sheetName().toLocal8Bit().data();
    #endif

                int i32Rows = workSheet->dimension().lastRow();
                int i32Columns = workSheet->dimension().lastColumn();

                if (i32Columns < 11)
                {
                    strErr.str("");
                    strErr << curSheetName << " => 列不合法应该为(11), columns=" << i32Columns;

                    UT_THROW_EXCEPTION(strErr.str());
                }

                unsigned int insideCode = 0, frameCode = 0;
                int frameSize = 0, beyond = 0;

                tmp = workSheet->read(1, 2);
                value = tmp.toString();
                frameSize = value.toInt(&Ok, 10);

                tmp = workSheet->read(1, 4);
                value = tmp.toString();
                frameCode = value.toInt(&Ok, 16);

                tmp = workSheet->read(1, 6);
                value = tmp.toString();
                beyond = value.toInt(&Ok, 10);

                if(beyond > frameSize)
                {
                    strErr.str("");
                    strErr << curSheetName << " => 帧长度异常，帧长度(" << frameSize << ")<(" << beyond << ")请检查配置文件";
                    UT_THROW_EXCEPTION(strErr.str());
                }

                std::vector<std::shared_ptr<subStorageType>> tmpStorage;

                for(int rowIndex = 3; rowIndex <= i32Rows; rowIndex++)
                {
                    int columnIndex = 1;

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
    #ifdef Utf8_Coding
                    pId = value.toUtf8().data();
    #else
                    pId = value.toLocal8Bit().data();
    #endif

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
    #ifdef Utf8_Coding
                    pName = value.toUtf8().data();
    #else
                    pName = value.toLocal8Bit().data();
    #endif

                    /// 遇到空内容则停止
                    if( (pId == "") && (pName == ""))
                    {
                        break;
                    }

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    byteStart = value.toInt(&Ok, 10);

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    byteSize = value.toInt(&Ok, 10);

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    bitStart = value.toInt(&Ok, 10);

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    bitSize = value.toInt(&Ok, 10);

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();

                    if(value.compare(QStringLiteral("是")) == 0)
                    {
                        bigSmal = BIGENDIAN;
                    }
                    else
                    {
                        bigSmal = SAMLLENDIAN;
                    }

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    if( value.contains("0x") || value.contains("0X"))
                    {
                        initData = value.toInt(&Ok, 16);
                    }
                    else
                    {
                        initData = value.toInt(&Ok, 10);
                    }

                    /* modify xqx 20200211 去除源码上下限，增加a b lsb
                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    srcMin = value.toInt(&Ok, 10);

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    srcMax = value.toInt(&Ok, 10);
                    */
                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    meanMin = value.toDouble(&Ok);

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    meanMax = value.toDouble(&Ok);

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
        #ifdef Utf8_Coding
                    pCategory = value.toUtf8().data();
        #else
                    pCategory = value.toLocal8Bit().data();
        #endif

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    d64A = value.toDouble(&Ok);

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    d64B = value.toDouble(&Ok);

                    tmp = workSheet->read(rowIndex, columnIndex++);
                    value = tmp.toString();
                    d64Lsb = value.toDouble(&Ok);

                    ///校验正确性
                    checkParamValid(curSheetName, frameSize, beyond, pId, pName, byteStart, byteSize, bitStart, bitSize, bigSmal, meanMin, meanMax, pCategory, d64A, d64B, d64Lsb);

                    tmpStorage.push_back(std::make_shared<subStorageType>(pId, pName, byteStart, byteSize, bitStart, bitSize, bigSmal, initData, meanMin, meanMax, pCategory, d64A, d64B, d64Lsb));
                }

                mParamsInfo.insert(decltype(mParamsInfo)::value_type(std::make_pair(frameCode, insideCode),
                                                                     std::make_tuple(frameCode, insideCode, frameSize, beyond, tmpStorage)
                                                                     ));
            }

        }

        void subProtocolConfigure::checkParamValid(std::string sheetName, int frameLen, int beyond, std::string pId, std::string pName, int byteStart, int byteSize, int bitStart, int bitSize, std::string bigSmal, double meanMin, double meanMax, std::string category, double a, double b, double lsb)
        {
            std::ostringstream strErr;

            if(pId == "")
            {
                strErr.str("");
                strErr << sheetName << " => 参数id为空，请检查配置文件";
                UT_THROW_EXCEPTION(strErr.str());
            }

            if( (byteStart < 0) || (byteSize < 0) || (bitStart < 0) || (bitSize < 0) )
            {
                strErr.str("");
                strErr << sheetName << " => 参数位置异常(" << std::dec << byteStart << "," << byteSize << "," << bitStart << "," << bitSize << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            if( (byteStart + byteSize + beyond ) > frameLen)
            {
                strErr.str("");
                strErr << sheetName << " => 参数位置(" << std::dec << byteStart << "," << byteSize << "," << bitStart << "," << bitSize << ")" << "大于帧长度(" << frameLen << ")异常";
                UT_THROW_EXCEPTION(strErr.str());
            }           

            if(meanMin > meanMax)
            {
                strErr.str("");
                strErr << sheetName << " => 工程值上下限异常(" << std::dec << meanMin << "," << meanMax << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }
        }

        std::string subProtocolConfigure::curProtocol()
        {
            return VAR_NAME(subProtocolConfigure);
        }

        bool subProtocolConfigure::isExist(unsigned int frameCode, unsigned int insideCode)
        {
            bool isFind = false;

            auto itor = mParamsInfo.find(std::make_pair(frameCode, insideCode));
            if(itor != mParamsInfo.end())
            {
                isFind = true;
            }

            return isFind;
        }

        int subProtocolConfigure::getMsgLen(unsigned int frameCode, unsigned int insideCode)
        {
            auto itor = mParamsInfo.find(std::make_pair(frameCode, insideCode));
            if(itor != mParamsInfo.end())
            {
                return std::get<subFrameLenIndex>((itor->second));
            }
            else
            {
                return -1;
            }
        }

        int subProtocolConfigure::getMsgBeyond(unsigned int frameCode, unsigned int insideCode)
        {
            auto itor = mParamsInfo.find(std::make_pair(frameCode, insideCode));
            if(itor != mParamsInfo.end())
            {
                return std::get<subBeyondIndex>((itor->second));
            }
            else
            {
                return -1;
            }
        }

        void subProtocolConfigure::getKeys(std::vector<std::pair<unsigned int, unsigned int>> &keys)
        {
            keys.clear();
            for(auto itor = mParamsInfo.begin(); itor != mParamsInfo.end(); itor++)
            {
                keys.push_back(itor->first);
            }
        }

        int subProtocolConfigure::getMaxByteSize(unsigned int frameCode, unsigned int insideCode)
        {
            auto itor = mParamsInfo.find(std::make_pair(frameCode, insideCode));
            if(itor != mParamsInfo.end())
            {
                auto storage = std::get<subStorageIndex>(itor->second);

                auto itor = std::max_element(storage.begin(), storage.end(),
                                             [=](const std::shared_ptr<subStorageType> first, const std::shared_ptr<subStorageType> second)
                {
                    return first->getMessage<sub_param_start_post_index>() < second->getMessage<sub_param_start_post_index>();
                });

                return (*itor)->getMessage<sub_param_start_post_index>() + (*itor)->getMessage<sub_param_byte_size_index>();
            }
            else
            {
                return -1;
            }
        }

        bool subProtocolConfigure::getStorages(std::vector<std::shared_ptr<subStorageType>> &outStorages, unsigned int frameCode, unsigned int insideCode)
        {
            bool res = false;

            auto itor = mParamsInfo.find(std::make_pair(frameCode, insideCode));
            if(itor != mParamsInfo.end())
            {
                res = true;

                auto storages = std::get<subStorageIndex>((itor->second));

                std::copy(storages.begin(), storages.end(), std::back_inserter(outStorages));
            }

            return res;
        }
    }
}
