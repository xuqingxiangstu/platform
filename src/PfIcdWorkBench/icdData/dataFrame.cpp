#include "dataFrame.h"
#include "datastorage.h"
#include "datacalc.h"

#include <algorithm>

#include "../../PfCommon/tools/ut_error.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        dataFrame::dataFrame()
        {

        }

        void dataFrame::parse(const std::vector<std::shared_ptr<subProtocol::subStorageType>> &storages, std::vector<icdOutConvertValueType> &outValues, const unsigned char *u8Msg, const unsigned int &u32Size, const int &beyond)
        {
            std::string paramsValue;
            dataStorage data;
            __int64 pValue;

            outValues.clear();

            dataCalc calc;

            for(auto subStorage : storages)
            {
                std::string pId = subStorage->getMessage<subProtocol::sub_param_id_index>();
                int byte_start = subStorage->getMessage<subProtocol::sub_param_start_post_index>();
                int byte_size = subStorage->getMessage<subProtocol::sub_param_byte_size_index>();
                int bit_start = subStorage->getMessage<subProtocol::sub_param_bit_start_pos_index>();
                int bit_size = subStorage->getMessage<subProtocol::sub_param_bit_size_index>();
                std::string bigSmall = subStorage->getMessage<subProtocol::sub_param_small_big_index>();

                pValue = data.getData(u8Msg, u32Size, byte_start + beyond, byte_size, bit_start, bit_size, bigSmall);

                 //根据原始数据换算工程值并进行计算(y = a * x * lsb + b)

                std::string strCategory = subStorage->getMessage<subProtocol::sub_param_category_index>();
                double d64A = subStorage->getMessage<subProtocol::sub_param_a_index>();
                double d64B = subStorage->getMessage<subProtocol::sub_param_b_index>();
                double d64Lsb = subStorage->getMessage<subProtocol::sub_param_lsb_index>();

                //int i32Precision = subStorage->getMessage<subProtocol::paramterInfo_precision_index>(info);
                int i32Precision = 3;

                /// 数据计算
                std::string calResult =  calc.getData(strCategory, pValue, d64A, d64B, d64Lsb, i32Precision);

                outValues.push_back(std::make_pair(pId, calResult));
            }
        }

        void dataFrame::simulation(unsigned char *outValue, const int &size, const std::vector<std::shared_ptr<subProtocol::subStorageType>> &inInfo,const int &beyond, const std::vector<icdInValueType> &inValue)
        {
            int calSize = 0;  ///< 帧总长度
            int tmpValue = 0;
            dataStorage data;
            std::ostringstream strErr;

            if(inInfo.size() == 0)
            {
                UT_SHOW("simulation 输入信息个数为0");
                return ;
            }

            if(outValue == nullptr)
            {
                UT_SHOW("simulation 输入首地址个数为nullptr");
                return ;
            }

            /// 计算总长度

            auto itor = std::max_element(inInfo.begin(), inInfo.end(),
                                           [=](const std::shared_ptr<subProtocol::subStorageType> a, std::shared_ptr<subProtocol::subStorageType> b)
            {
                return a->getMessage<subProtocol::sub_param_start_post_index>() < b->getMessage<subProtocol::sub_param_start_post_index>();
            });

            calSize = (*itor)->getMessage<subProtocol::sub_param_start_post_index>() + (*itor)->getMessage<subProtocol::sub_param_byte_size_index>();

            if(calSize > size)
            {
                strErr << "总长度(" << std::dec << size << ")小于 待填充字节(" << calSize << ")长度!";
                UT_THROW_EXCEPTION(strErr.str());
            }

            for(auto info : inInfo)
            {
                std::string pId = info->getMessage<subProtocol::sub_param_id_index>();
                int byte_start = info->getMessage<subProtocol::sub_param_start_post_index>();
                int byte_size = info->getMessage<subProtocol::sub_param_byte_size_index>();
                int bit_start = info->getMessage<subProtocol::sub_param_bit_start_pos_index>();
                int bit_size = info->getMessage<subProtocol::sub_param_bit_size_index>();
                std::string bigSmall = info->getMessage<subProtocol::sub_param_small_big_index>();
                tmpValue = 0;

                auto findItor = std::find_if(inValue.begin(), inValue.end(),
                                             [=](const icdInValueType v)
                                                {
                                                    return std::get<0>(v) == pId;
                                                }
                                             );
                if(findItor != inValue.end())
                {
                    tmpValue = std::get<1>(*findItor);
#if 0
                    strErr.str("");
                    strErr << pId << " => " << std::dec << (int)tmpValue << "/0x" << std::hex << (int)tmpValue;
                    SHOW(strErr.str());
#endif
                }

                try
                {
                    data.setData(outValue, size, byte_start + beyond, byte_size, bit_start, bit_size, tmpValue, bigSmall);
                }
                catch(std::runtime_error err)
                {
                    strErr << "id(" << pId << ") " << err.what();
                    UT_THROW_EXCEPTION(strErr.str());
                }
            }
        }
    }
}

