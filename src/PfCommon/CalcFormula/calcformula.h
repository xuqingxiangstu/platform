#ifndef CALCFORMULA_H
#define CALCFORMULA_H
#include<stack>
#include<math.h>
#include<map>
#include<string>
#include<queue>
#include<iostream>
#include<sstream>
#include "calcformula_global.h"
#define CALC_SYMBOL_MAX_SIZE        4
/**
 * @file calcformula.h
 * @brief The CalcFormula class
 * 公式计算，支持各种符号，相当于计算器的功能
 * @author fjt
 * @date 2020-02-20
 * @version V1.0
 */
namespace Pf
{
    namespace PfCommon
    {
        class CALCFORMULASHARED_EXPORT CalcFormula
        {
            private:
            unsigned int mcalcSymbolMaxLen;
            void InitCalcSymbolMap();
            public:
            /*静态成员初始化状态*/
            static bool mbInitFlag;
            /**运算符集合 保存运算符和优先级（0-99 数字越大优先级越高）**/
            static std::map<std::string,int> mCalcSymbol;

            std::string mFormula;
            /**计算结果**/
            std::string mstrResult;/// 字符串结果
            long long mInt64Result; /// 64位整型结果
            int mType;       ///类型
            double mdResult; ///double类型的结果
            /**
            * @brief isDigitOrDoit    判断是否为数字或者小数点
            * @param c 字符
            * @return true表示是，false表示否
            */
            bool isDigitOrDoit(char c);
            /**
            * @brief isDigitOrDoit 判断是否为数字或者小数点
            * @param c 字符串
            * @return true表示是，false表示否
            */
            bool isDigitOrDoit(std::string c);
            /**
            * @brief isDigitHex 判断是否为16进制
            * @param str 字符串
            * @return true表示是，false表示否
            */
            bool isDigitHex(std::string str);
            /**
            * @brief isSpace 判断是否为空格
            * @param c 字符
            * @return true表示是，false表示否
            */
            bool isSpace(char c);
            /**
            * @brief isSpace
            * @param c字符串
            * @return true表示是，false表示否
            */
            bool isSpace(std::string c);
            /**
            * @brief isSymbol 判断是否为符号
            * @param c为字符
            * @return  true表示是，false表示否
            */
            bool isSymbol(char c);
            /**
            * @brief isSymbol 判断是否为符号
            * @param c字符串
            * @return true表示是，false表示否
            */
            bool isSymbol(std::string c);
            /**
            * @brief isSign 判断是否为正负号
            * @param c字符
            * @return  true表示是，false表示否
            */
            bool isSign(char c);
            /**
            * @brief isSign 判断是否为正负号
            * @param c字符串
            * @return true表示是，false表示否
            */
            bool isSign(std::string c);

            /**
            * @brief isOperator 判断是否为运算符
            * @param s 字符
            * @return true表示是，false表示否
            */
            bool isOperator(char s);
            /**
            * @brief isOperator 判断是否为运算符
            * @param s 字符串
            * @return true表示是，false表示否
            */
            bool isOperator(std::string s);

            /**
            * @brief isNumber 判断是否为数字
            * @param s字符串
            * @return true表示是，false表示否
            */
            bool isNumber(std::string s);

            /**
            * @brief isInt 判断是否为整数
            * @param s 字符串
            * @return true表示是，false表示否
            */
            bool isInt(std::string s);

            /**
            * @brief isDouble 判断是否为浮点数
            * @param s字符串
            * @return true表示是，false表示否
            */
            bool isDouble(std::string s);

            /**
            * @brief isLeftBracket 判断是否为左括号
            * @param s字符串
            * @return true表示是，false表示否
            */
            bool isLeftBracket(std::string s);

            /**
            * @brief isRightBracket 判断是否为右括号
            * @param s字符串
            * @return true表示是，false表示否
            */
            bool isRightBracket(std::string s);

            /**
            * @brief SignPriority 计算符号优先级
            * @param s字符串
            * @return 优先级级别，值越大优先级越大
            */
            int  SignPriority(std::string s);

            /**
            * @brief BracketMatch 括号匹配
            * @param queue 存放符号的队列
            * @return true表示匹配成功，false表示匹配失败
            */
            bool BracketMatch(std::queue<std::string> queue);

            /**
            * @brief transform 表达式转化
            * @param exp 表达式
            * @param output 转换结果
            * @return true表示成功转换，false表示转换失败
            */
            bool transform(std::queue<std::string> exp, std::queue<std::string> &output);

            /**
            * @brief SplitExpress 表达式分割
            * @param exp 表达式字符串
            * @param output 分隔输出
            * @return true表示分割成功，false表示分割失败
            */
            bool SplitExpress(const std::string &exp,std::queue<std::string> &output);

            /**
            * @brief Calculate 表达式计算
            * @param exp表达式字符串
            * @return 计算结果
            */
            std::string Calculate( std::queue<std::string> exp);
            /**
            * @brief Calculate表达式计算
            * @param l 左操作数
            * @param op 操作符
            * @param r 右操作数
            * @return 计算结果
            */
            std::string Calculate( std::string l,std::string op, std::string r);
            /**
            * @brief Replacestr 替换指定字符
            * @param orignStr 源字符串
            * @param oldStr 源字符串中要替换的字符串
            * @param newStr 替换的成新字符串的内容
            * @return
            */
            std::string Replacestr(const std::string& orignStr,const std::string& oldStr,const std::string& newStr);
            public:
            CalcFormula();
            ~CalcFormula();
            /**
            * @brief expression 对表达式进行计算，并判断计算是否正确
            * @param exp 表达是字符串
            * @return true表示计算结果正常，false表示计算结果错误
            */
            bool expression(const std::string& exp);

            /**
            * @brief getResult 获取计算结果
            * @param ret 获取int类型的结果
            * @return 获取成功
            */
            bool getResult(int &ret)
            {
               if(1==mType)
               {
                   ret = (int)mInt64Result;
                   return true;
               }else
                   return false;
            }
            /**
            * @brief getResult 获取结果
            * @param ret 获取double类型的结果
            * @return 获取成功
            */
            bool getResult(double &ret)
            {
               if(2==mType)
               {
                   ret = mdResult;
                   return true;
               }else
                   return false;
            }
            /**
            * @brief getType 获取计算类型
            * @return 1表示int，2表示double
            */
            int getType();
            /**
            * @brief result 计算结果
            * @return 计算结果
            */
            std::string result();

        };
    }
}
#endif // CALCFORMULA_H
