#ifndef DATA_H
#define DATA_H

#include <string>

#define B_THAN          ">"
#define B_THAN_EQUAL    ">="
#define B_EQUAL         "=="
#define B_LESS          "<"
#define B_LESS_EQUAL    "<="
#define B_WITH          "&&"

using dataValueType = double;

class calcFactory
{
public:
    /**
     * @brief calc      公式计算
     * @param condition
     * @param bV
     * @return
     */
    static bool calc(const std::string &condition, const dataValueType &inV);

    static bool judge(const std::string &condition, const dataValueType &inV, const dataValueType &bV);
};

class data
{
public:

    typedef enum
    {
        Hex,
        Dec,
        Float
    }dataType;

    /**
     * @brief getIndexFromV 通过字符串获取三级索引 !1@2#23$Value$4
     * @param src
     * @param level1Index
     * @param level2Index
     * @param level3Index
     * @return
     */
    static bool getIndexFromV(const std::string &src, std::string &level1Index, std::string &level2Index, std::string &level3Index
                              ,std::string &var, std::string &value);

    /**
     * @brief isDigital 校验字符串是否是数字
     * @param src
     * @param isOk
     * @return Hex:十六进制 Dec：十进制
     */
    static dataType isDigital(const std::string &src, bool &isOk);

    /**
     * @brief converDigital 字符串转数字
     * @param src
     * @param value
     * @return
     */
    static bool converDigital(const std::string &src, dataValueType &value);

    /**
     * @brief getCondAndBV      获取比较条件及比较数据
     * @param src
     * @param condition
     * @param BV
     * @return
     */
    static bool getSymboolAndBV(const std::string &src, std::string &symbool, dataValueType &BV);

    /**
     * @brief beyondSymboolIsOk 校验比较条件是否满足
     * @param src
     * @return
     */
    static bool isBeyondSymboolOk(const std::string &src);

    /**
     * @brief isOkCalcFormat        校验Calc格式是否正确
     * @param src
     * @return
     */
    static bool isOkCalcFormat(const std::string &src);
};

#endif // DATA_H
