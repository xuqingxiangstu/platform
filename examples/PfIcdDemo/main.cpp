#include <QCoreApplication>

#include <iostream>
#include <vector>

#include <QDebug>
#include "../../src/PfSql/paramsTable/paramstable.h"
#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../../src/PfCommon/crc/crc.h"
using namespace Pf;
#pragma execution_character_set("utf-8")
/**
 * @brief 帧解析与仿真例程
 * @example PfIcdDemo/main.cpp
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::shared_ptr<PfIcdWorkBench::icdFrameAdapter> frameAdapter = std::make_shared<PfIcdWorkBench::icdFrameAdapter>();

    try
    {
        //初始化sqlite
        paramsTable::getInstance();

        //step2：初始化帧解析xml
        frameAdapter->init("./cfgfile/icd.xml");

        qDebug() << "init Ok\n";

        //step3：获取待解析的帧句柄
        auto obj = frameAdapter->getFrameObj("FE");

        if(obj != nullptr)
        {
#if 0
            std::vector<unsigned char> msg = {0xAA,
                                              0x0, 0x42,
                                              0x0,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0, 0x0, 0x0, 0x0,                                              
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0,
                                              0x0,
                                              0x1,
                                              0x0, 0x3,
                                              0x0, 0x0, 0x0, 0x0,
                                              0xF0,

                                              0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC,
                                              0x2, 0x3, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC,
                                              0x3, 0x4, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC
                                             };
#else
            std::vector<unsigned char> msg = {0xAA,
                                              0x0, 0x42,
                                              0x0,
                                              0x1, 0x11, 0x22, 0x33,
                                              0x2, 0x22, 0x33, 0x44,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0,
                                              0x0,
                                              0x3,
                                              0x0, 0x3,
                                              0x0, 0x0, 0x0, 0x0,
                                              0xF0
                                             };


            std::vector<unsigned char> infoMsg1 = {0x10, 0x42,
                                                   0x0, 0x1,
                                                   0x0,
                                                   0x0,
                                                   0x1,
                                                   0x0,
                                                   0x0, 0x0, 0x0, 0x0,
                                                   0x0, 0
            };

            /*std::string tmp1 = "hello word";
            infoMsg1.push_back(tmp1.size());

            std::copy(tmp1.begin(), tmp1.end(), std::back_inserter(infoMsg1));
*/
            std::copy(infoMsg1.begin(), infoMsg1.end(), std::back_inserter(msg));

            std::vector<unsigned char> infoMsg2 = {0x10, 0x42,
                                                   0x0, 0x2,
                                                   0x0,
                                                   0x0,
                                                   0x3,
                                                   0x0,
                                                   0x0, 0x0, 0x0, 0x0,
                                                   0x0, 0
            };

            std::string tmp2 = "ni hao ceshi li cheng";
            infoMsg2.push_back(tmp2.size());

            std::copy(tmp2.begin(), tmp2.end(), std::back_inserter(infoMsg2));

            std::copy(infoMsg2.begin(), infoMsg2.end(), std::back_inserter(msg));

            std::vector<unsigned char> infoMsg3 = {0x42, 0x10,
                                                   0x0, 0x3,
                                                   0x0,
                                                   0x0,
                                                   0x3,
                                                   0x0,
                                                   0x0, 0x0, 0x0, 0x0,
                                                   0x0, 0
            };

            std::string tmp3 = "std::copy(tmp2.begin(), tmp2.end(), std::back_inserter(infoMsg2))";
            infoMsg3.push_back(tmp3.size());

            std::copy(tmp3.begin(), tmp3.end(), std::back_inserter(infoMsg3));

            std::copy(infoMsg3.begin(), infoMsg3.end(), std::back_inserter(msg));

            int len = msg.size() - 4;

            msg.at(1) = (len >> 8) & 0xFF;
            msg.at(2) = len & 0xFF;

            msg.at(33) = Pf::PfCommon::Crc::calSum(&msg.at(34), msg.size() - 34);

            std::cout << msg.size() << std::endl;

            for(auto v : msg)
            {
                //std::cout << std::hex << (int)v << " ";
            }

           // std::cout << std::endl;

#endif

#if 1
            //step4：根据Excel中的数据进行仿真
            PfIcdWorkBench::byteArray sendMsg;
#define TEST 4
#if TEST == 0
            std::string msgJson = "{\
                                  \"head\":\
                                  {\
                                      \"head_frame_type\":0,\
                                      \"head_src_sys_type\":1,\
                                      \"head_src_sys_code\":2,\
                                      \"head_src_node_code\":3,\
                                      \"head_dst_sys_type\":11,\
                                      \"head_dst_sys_code\":22,\
                                      \"head_dst_node_code\":33,\
                                      \"head_info_word_type\":3\
                                  },\
                                  \"infoWord\":\
                                  [    \
                                      {\
                                          \"info_3_table_num\":16912,\
                                          \"info_3_code\":1,\
                                          \"info_3_dev_num\":11,\
                                          \"info_3_module\":22,\
                                          \"info_3_data_type\":1,\
                                          \"info_3_over\":0,\
                                          \"info_3_data_value\":0.123,\
                                          \"info_3_reserve\":0  \
                                      },\
                                      {\
                                          \"info_3_table_num\":16912,\
                                          \"info_3_code\":2,\
                                          \"info_3_dev_num\":22,\
                                          \"info_3_module\":33,\
                                          \"info_3_data_type\":3,\
                                          \"info_3_over\":0,\
                                          \"info_3_data_value\":3.6,\
                                          \"info_3_reserve\":0,\
                                          \"info_3_string_len\":8,\
                                          \"info_3_string_value\":\"hellword\"\
                                      },\
                                      {\
                                          \"info_3_table_num\":16912,\
                                          \"info_3_code\":3,\
                                          \"info_3_dev_num\":33,\
                                          \"info_3_module\":44,\
                                          \"info_3_data_type\":3,\
                                          \"info_3_over\":0,\
                                          \"info_3_data_value\":1.69,\
                                          \"info_3_reserve\":0,\
                                          \"info_3_string_len\":12,\
                                          \"info_3_string_value\":\"12hellword34\"\
                                      }\
                                  ]\
                              }";
#elif TEST == 0
            std::string msgJson = "{\
                \"head\":\
                {\
                    \"head_frame_type\":0,\
                    \"head_src_sys_type\":1,\
                    \"head_src_sys_code\":2,\
                    \"head_src_node_code\":3,\
                    \"head_dst_sys_type\":11,\
                    \"head_dst_sys_code\":22,\
                    \"head_dst_node_code\":33,\
                    \"head_info_word_type\":2\
                },\
                \"infoWord\":\
                [    \
                    {\
                        \"info_2_table_num\":16896,\
                        \"info_2_code\":1,         \
                        \"info_2_reserve\":0  \
                    }\
                ],\
                \"region\":\
                {\
                    \"table_num\":16896,\
                    \"data\":\
                    [\
                        {\
                            \"coding\":1,\
                            \"value\":11   \
                        },\
                        {\
                            \"coding\":2,\
                            \"value\":3  \
                        },\
                        {\
                            \"coding\":3,\
                            \"value\":\"wer\"   \
                        },\
                        {\
                            \"coding\":4,\
                            \"value\":5   \
                        },\
                        {\
                            \"coding\":5,\
                            \"value\":\"yuiew\" \
                        }\
                    ]\
                }\
            }\
                                  ";
#elif TEST == 3
            std::string msgJson = "{\
                \"region\":\
                {\
                    \"table_num\":16896,\
                    \"data\":\
                    [\
                        {\
                            \"coding\":1,\
                            \"value\":11   \
                        },\
                        {\
                            \"coding\":2,\
                            \"value\":3  \
                        },\
                        {\
                            \"coding\":3,\
                            \"value\":\"wer\"   \
                        },\
                        {\
                            \"coding\":4,\
                            \"value\":5   \
                        },\
                        {\
                            \"coding\":5,\
                            \"value\":\"yuiew\" \
                        }\
                    ]\
                }\
            }\
                                  ";
#elif TEST == 4
            std::string msgJson = "{\
                  \"head\":\
                  {\
                      \"head_src_node\":2,\
                      \"head_dst_node\":3,\
                      \"data_type\":12,\
                      \"ack\":1,\
                      \"retrans_cnt\":2\
                  },\
                  \"region\":\
                  {\
                      \"table_num\":16896,\
                      \"data\":\
                      [\
                          {\
                              \"coding\":1,\
                              \"value\":11   \
                          },\
                          {\
                              \"coding\":2,\
                              \"value\":3  \
                          },\
                          {\
                              \"coding\":3,\
                              \"value\":\"wer\"   \
                          },\
                          {\
                              \"coding\":4,\
                              \"value\":5   \
                          },\
                          {\
                              \"coding\":5,\
                              \"value\":\"yuiew\" \
                          }\
                      ]\
                  }\
              }\
                                  ";
#endif
            obj->simulation(sendMsg, msgJson);
            qDebug() << "仿真测试->";

            for(auto v : sendMsg)
            {
                std::cout << std::hex << (int)v << " ";
            }
            std::cout << "\n";

//#else
            //step5：根据Excel中的数据进行解析
            std::vector<PfIcdWorkBench::icdOutConvertValueType> outV;
            qDebug() << "解析测试->\n";

            std::string jsonStr = obj->parse(&sendMsg.at(0), sendMsg.size());

            std::cout << jsonStr << std::endl;

            //打印解析后的数据 ID + Value
//            for(auto v : outV)
            {
                //std::cout << std::get<0>(v) << " " << std::get<1>(v) << std::endl;
            }
#endif
        }
        else
        {
            qDebug() << "get error";
        }

    }
    catch(std::runtime_error err)
    {
        //std::cout << err.what() << std::endl;
        qDebug() << err.what();
    }

    return a.exec();
}
