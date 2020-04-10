#include "calcformula.h"

#include <stdio.h>

namespace Pf
{
    namespace PfCommon
    {
        bool CalcFormula::mbInitFlag = false;
        std::map<std::string,int> CalcFormula::mCalcSymbol;
        CalcFormula::CalcFormula()
        {
            InitCalcSymbolMap();
            mstrResult ="";
            mFormula="";
            mcalcSymbolMaxLen =4;
        }
        CalcFormula::~CalcFormula()
        {
            mFormula = "";
            mstrResult = "";
        }
        void CalcFormula::InitCalcSymbolMap()
        {
            if(!mbInitFlag)
                  {
                      mCalcSymbol.clear();
                      /*逻辑运算*/
                      /****************************************/
                      /*逻辑或*/
                      mCalcSymbol.insert(std::make_pair("||",8));
                      /*逻辑与*/
                      mCalcSymbol.insert(std::make_pair("&&",9));
                      /*逻辑非*/
                      mCalcSymbol.insert(std::make_pair("!",10));

                      /****************************************/
                      /*大小比较运算*/
                      /**************************************/
                      /*大于*/
                      mCalcSymbol.insert(std::make_pair(">",9));
                      /*大于等于*/
                      mCalcSymbol.insert(std::make_pair(">=",9));
                      /*小于*/
                      mCalcSymbol.insert(std::make_pair("<",9));
                      /*小于等于*/
                      mCalcSymbol.insert(std::make_pair("<=",9));
                      /*等于*/
                      mCalcSymbol.insert(std::make_pair("==",9));
                      /*不等于*/
                      mCalcSymbol.insert(std::make_pair("!=",9));
                      /**************************************/
                      /*位或运算*/
                      mCalcSymbol.insert(std::make_pair("|",10));
                      /*位与运算*/
                      mCalcSymbol.insert(std::make_pair("&",10));
                      /*位取反*/
                      mCalcSymbol.insert(std::make_pair("~",10));
                      /*左移运算*/
                      mCalcSymbol.insert(std::make_pair("<<",11));
                      /*右移运算*/
                      mCalcSymbol.insert(std::make_pair(">>",11));

                      /*加运算*/
                      mCalcSymbol.insert(std::make_pair("+",12));
                      /*减运算*/
                      mCalcSymbol.insert(std::make_pair("-",12));
                      /*乘法运算*/
                      mCalcSymbol.insert(std::make_pair("*",13));
                      /*除法运算*/
                      mCalcSymbol.insert(std::make_pair("/",13));
                      /*取模运算*/
                      mCalcSymbol.insert(std::make_pair("%",13));
                      /*取模运算*/
                      mCalcSymbol.insert(std::make_pair("MOD",13));
                      /*指数运算*/
                    //  mCalcSymbol.insert(std::make_pair("POW",14));
                      /*最小值运算必须加括号*/
                    //  mCalcSymbol.insert(std::make_pair("MIN",14));
                      /*最大值运算必须加括号*/
                   //   mCalcSymbol.insert(std::make_pair("MAX",14));
                      /*异或运算*/
                      mCalcSymbol.insert(std::make_pair("^",14));
                      /*整数绝对值*/
                      mCalcSymbol.insert(std::make_pair("ABS",14));
                      /*浮点绝对值*/
                      mCalcSymbol.insert(std::make_pair("FABS",14));
                      mbInitFlag = true;
                  }
                  std::map<std::string,int>::iterator iter;
                  for(iter = mCalcSymbol.begin();iter!=mCalcSymbol.end();iter++)
                  {
                      std::string strtemp = iter->first;
                      if(strtemp.size()>mcalcSymbolMaxLen)
                          mcalcSymbolMaxLen = strtemp.size();
                  }
        }
        bool CalcFormula::isDigitHex(std::string str)
        {
            bool isHex= false;
            int len = str.size();
            for(int i=0;i<len;i++)
            {
                if((str.at(i)<'0')||(str.at(i)>L'9'))
                {
                    if((str.at(i)!='.')&&(str.at(i)!=' '))
                    {
                        if(((str.at(i)>='a')&&(str.at(i)<='f'))||((str.at(i)>='A')&&(str.at(i)<='F'))||(str.at(i)=='x')||(str.at(i)=='X'))
                        {
                            isHex = true;
                            break;
                        }
                    }
                }
            }
            return isHex;
        }
        /*判断是否为数字或者小数点或者16进制数*/
        bool CalcFormula::isDigitOrDoit(char c)
        {
            bool ret = false;
            ret = (iswdigit(c)||('.'==c)||((c>='a')&&(c<='f'))||((c>='A')&&(c<='F'))||(c=='x')||(c=='X'));
            return ret;
        }
        /*判断是否为空格*/
        bool CalcFormula::isSpace(char c)
        {
            char space = ' ';
            bool ret = false;
            if(space == c){
                ret = true;
            }
            return ret;
        }
        bool CalcFormula::isSpace(std::string c)
        {
            std::string space = " ";
            bool ret = false;
            if(space == c){
                ret = true;
            }
            return ret;
        }
        bool CalcFormula::isDigitOrDoit(std::string c)
        {
            std::stringstream strtmp(c);
            double d;
            int i;
            /*浮点型*/
            if(strtmp>>d)
                return true;
            /*整形*/
            if(strtmp>>i)
                return true;
            return false;
        }
        /*判断是否为正负号*/
        bool CalcFormula::isSign(char c)
        {
            return ( (c=='+')||(c =='-') );
        }
        bool CalcFormula::isSign(std::string c)
        {
            return ( (c=="+")||(c =="-") );
        }
        /*判断是否为符号*/
        bool CalcFormula::isSymbol(char c)
        {
                return ( isOperator(c) || (c=='(') || (c==')'));
        }
        bool CalcFormula::isSymbol(std::string c)
        {
                return ( isOperator(c) || (c=="(") || (c==")") );
        }
        /*判断是否为运算符*/
        bool CalcFormula::isOperator(char s)
        {
            std::string ss=" ";
            ss.at(0)=s;
            return isOperator(ss);
        }
        bool CalcFormula::isOperator(std::string s)
        {
               std::map<std::string,int>::iterator iter;
                iter = mCalcSymbol.find(s);
                if(iter==mCalcSymbol.end())
                    return false;
                else
                    return true;
        }
        /*计算符号优先级*/
        int  CalcFormula::SignPriority(std::string s)
        {
           std::map<std::string,int>::iterator iter;
            iter = mCalcSymbol.find(s);
            if(iter==mCalcSymbol.end())
                return 0;
            else
                return iter->second;
        }
         /*判断是否为数字*/
        bool CalcFormula::isNumber(std::string s)
        {
              std::stringstream wss(s);
              double d;
              int i;


              if((wss>>d)||(wss>>i)||(wss>>std::hex>>i))
                    return true;
              else
                    return false;
         }
        /*判断是否为整数*/
        bool CalcFormula::isInt(std::string s)
        {
            if(!isNumber(s))
                return false;
            if(isDigitHex(s))
                return true;
            for(unsigned int i=0;i<s.size();i++)
            {
                if(s.at(i)=='.')
                {
                    return false;
                }
            }
            return true;
         }
        /*判断是否为浮点数*/
        bool CalcFormula::isDouble(std::string s)
        {
            if(!isNumber(s))
                return false;
            if(isDigitHex(s))
                return false;
            for(unsigned int i=0;i<s.size();i++)
            {
                if(s.at(i)=='.')
                {
                    return true;
                }
            }
            return false;
        }
        /*是否为左括号*/
        bool CalcFormula::isLeftBracket(std::string s)
        {
            return (s == "(" );
        }
        /*是否为右括号*/
        bool CalcFormula::isRightBracket(std::string s)
        {
            return (s ==")" );
        }
        /*括号匹配检查*/
        bool CalcFormula::BracketMatch(std::queue<std::string> queuestr)
        {
            bool ret = true;
            std::stack<std::string> stack_braket;

            int length = queuestr.size();
            for(int i=0;i<length; i++)
            {
                if( isLeftBracket(queuestr.front()))
                {
                    stack_braket.push(queuestr.front());
                }
                else if( isRightBracket(queuestr.front()) )
                {
                    if( (!stack_braket.empty()) &&  isLeftBracket(stack_braket.top()) )
                    {
                        stack_braket.pop();
                    }
                    else
                    {
                        ret = false;
                        break;
                    }
                }
                queuestr.pop();
            }
            return ret&stack_braket.empty();
        }
        /*表达式分割*/
        bool CalcFormula::SplitExpress(const std::string &exp,std::queue<std::string> &queueVal)
        {
            int exp_length = 0;
            std::string str_num;
            //std::string str_calc_str;
            std::string str_pre;
            while(queueVal.size()>0)
            {
                queueVal.pop();
            }
            exp_length = exp.size();
            for(int i=0; i<exp_length; i++)
            {
                /*空格*/
                if( isSpace(exp[i]) )
                {
                    continue;
                }
                /*数字或者小数点*/
                if(('A'== exp[i])||('B'==exp[i])||('C'== exp[i])||('D'==exp[i])||('E'== exp[i])||('F'==exp[i]))
                {
                        std::string calc_str;
                         bool findCalc_Symbol = false;
                        for(unsigned int j = mcalcSymbolMaxLen;j>0;j--)
                        {
                            while(i+j>exp.size())
                            {
                                j = j-1;
                                if(j<1){
                                   /*find symbol error*/
                                  break;
                                }
                            }
                            calc_str = exp.substr(i,j);
                            if(isSymbol(calc_str))
                            {
                                if(!str_num.empty())
                                {
                                    queueVal.push(str_num);/*数值写入队列*/
                                    str_num.clear();
                                }
                                if( isSign(calc_str) && ( (str_pre=="")||(str_pre=="(")||(isOperator(str_pre))) )
                                {
                                    str_num += calc_str;
                                }
                                else
                                {
                                    if(("~"==calc_str)||("!")==calc_str||("ABS"==calc_str)||("FABS"==calc_str))
                                   {
                                       queueVal.push("0");//zxc 20190419单目运算符补0
                                   }
                                   queueVal.push(calc_str);
                                }
                                str_pre = calc_str;
                                findCalc_Symbol = true;
                                i+=j-1;
                                break;
                            }
                        }
                        if(findCalc_Symbol){
                            continue;
                        }
                }
                if( isDigitOrDoit(exp[i]) )
                {
                    str_num += exp[i];
                    str_pre = exp[i];
                }
                else/*字符运算符或者符号*/
                {
                    std::string calc_str;
                    bool findCalc_Symbol = false;
                    for(unsigned int j = mcalcSymbolMaxLen;j>0;j--)
                    {
                        while(i+j>exp.size())
                        {
                            j = j-1;
                            if(j<1){
                               /*find symbol error*/
                              return false;
                            }
                        }
                        calc_str = exp.substr(i,j);
                        if(isSymbol(calc_str))
                        {
                            if(!str_num.empty())
                            {
                                queueVal.push(str_num);/*数值写入队列*/
                                str_num.clear();
                            }
                            if( isSign(calc_str) && ( (str_pre=="")||(str_pre=="(")||(isOperator(str_pre))) )
                            {
                                str_num += calc_str;
                            }
                            else
                            {
                                if(("~"==calc_str)||("!")==calc_str||("ABS"==calc_str)||("FABS"==calc_str))
                               {
                                   queueVal.push("0");//zxc 20190419单目运算符补0
                               }
                               queueVal.push(calc_str);
                            }
                            str_pre = calc_str;
                            findCalc_Symbol = true;
                            i+=j-1;
                            break;
                        }
                    }
                    if(!findCalc_Symbol)
                    {
                        return false;
                    }
                }

            }

            if(!str_num.empty())
            {
                queueVal.push(str_num);
            }
            return true;
        }
        /*表达式转换*/
        bool CalcFormula::transform(std::queue<std::string> exp, std::queue<std::string>& output)
        {
            bool ret = BracketMatch(exp);
            std::stack<std::string> stack_str;
            while(stack_str.size()>0)
            {
                stack_str.pop();
            }
            while(output.size()>0)
            {
                output.pop();
            }
            while( ret &&(!exp.empty()))
            {
                std::string e = exp.front();
                exp.pop();
                /*判断是否为数字*/
                if( isNumber(e) )
                {
                    output.push(e);
                }
                else if( isOperator(e) )
                {
                    while( !stack_str.empty() && (SignPriority(e) <= SignPriority(stack_str.top())) &&(isOperator(stack_str.top())))
                    {
                        /*将优先级高的运算符压入堆栈*/
                        output.push(stack_str.top());
                        stack_str.pop();
                    }
                    /*将低优先级运算符压入对战*/
                    stack_str.push(e);
                }
                else if( isLeftBracket(e) )
                {
                    stack_str.push(e);
                }
                else if( isRightBracket(e) )
                {
                    while( (!stack_str.empty()) && (!isLeftBracket(stack_str.top())) )
                    {
                        output.push(stack_str.top());
                        stack_str.pop();
                    }

                    if( !stack_str.empty() )
                    {
                        stack_str.pop();
                    }
                }
                else
                {
                    ret = false;
                }
            }

            while( !stack_str.empty() )
            {
                output.push(stack_str.top());
                stack_str.pop();
            }
            if( !ret )
            {
                while(output.size()>0)
                {
                    output.pop();
                }
            }
            return ret;
        }
        /*表达式计算*/
        std::string CalcFormula::Calculate(std::queue<std::string> exp)
        {
            std::string ret = "Error";
            std::stack<std::string> stack_str;
            while(!exp.empty())
            {
                std::string e = exp.front();
                        exp.pop();

                if( isNumber(e) )
                {
                    stack_str.push(e);
                }
                else if(isOperator(e))
                {
                    std::string rp = "";
                    std::string lp = "";
                    if(!stack_str.empty())
                    {
                        rp = stack_str.top();
                        stack_str.pop();
                    }
                    if(!stack_str.empty())
                    {
                        lp = stack_str.top();
                        stack_str.pop();
                    }
                    std::string result = Calculate(lp,e,rp);
                    if(result != "Error" )
                    {
                       stack_str.push(result);
                    }
                    else
                    {
                       break;
                    }
                }
                else
                {
                  break;
                }
            }

            if(exp.empty() && (stack_str.size() == 1) && isNumber(stack_str.top()) )
            {
              ret = stack_str.top();
              stack_str.pop();
            }

            return ret;
        }
        /*计算*/
        std::string CalcFormula::Calculate(std::string l,std::string op,std::string r)
        {
            std::string ret = "Error";
            char tmpData[64];
            if( isNumber(l) && isNumber(r) )
            {
                double lp = 0;
                double rp = 0;
                unsigned int v_uintTmp=0;
                std::stringstream sinl;
                std::stringstream sinr;
                std::ostringstream oss;
                sinl<<l;
                if((!(sinl>>lp))||isDigitHex(l))
                {
                    scanf(l.c_str(),"%x",&v_uintTmp);
                    lp = v_uintTmp;
                }
                sinr<<r;
                if((!(sinr>>rp))||isDigitHex(r))
                {
                    scanf(r.c_str(),"%x",&v_uintTmp);
                    rp = v_uintTmp;
                }
                if(op == "+")/*加法运算*/
                {
                    if(isInt(l)&&isInt(r))
                    {
                        mType = 1;
                        mInt64Result= (long long)(lp+rp);
                        oss<<mInt64Result;
                        ret = oss.str();
                    }else{
                        mType = 2;
                        mdResult = lp+rp;
                        sprintf(tmpData,"%.9f",mdResult);
                        ret = tmpData;
                    }
                }
                else if( op == "-")/*减法运算*/
                {
                    if(isInt(l)&&isInt(r))
                    {
                        mType = 1;
                        mInt64Result= (long long)(lp-rp);
                        oss<<mInt64Result;
                        ret = oss.str();
                    }else{
                        mType = 2;
                        mdResult = lp-rp;
                        sprintf(tmpData,"%.9f",mdResult);
                        ret = tmpData;
                    }
                }
                else if( op == "*")/*乘法运算*/
                {
                    if(isInt(l)&&isInt(r))
                    {;
                        mType = 1;
                        mInt64Result= (long long)(lp*rp);
                        oss<<mInt64Result;
                        ret = oss.str();
                    }else{
                        mType = 2;
                        mdResult = lp*rp;
                        sprintf(tmpData,"%.9f",mdResult);
                        ret = tmpData;
                    }
                }
                else if( op == "/")/*除法运算*/
                {
                    const double P = 0.000000000000001;

                    if( (rp > -P) && (rp < P) )
                    {
                        ret = "Error";
                    }
                    else
                    {
                        mType = 2;
                        mdResult = lp / (rp*1.0);
                        sprintf(tmpData,"%.9f",mdResult);
                        ret = tmpData;
                    }
                }/* else if( op == "MIN")//最小值运算
                {
                    mType = 2;
                    mdResult = min(lp,rp);
                    sprintf(tmpData,64,"%.9f",mdResult);
                    ret = tmpData;
                } else if( op == "MAX")//最大值运算
                {
                    mType = 2;
                    mdResult = max(lp,rp);
                    sprintf(tmpData,64,"%.9f",mdResult);
                    ret = tmpData;
                }*/
                else if( op == "ABS")/*绝对值运算*/
                {
                    std::stringstream sinr;
                    int rInt = 0;
                    sinr.clear();
                    sinr<<r;
                    if((!(sinr>>rInt))||isDigitHex(r))
                    {
                        scanf(r.c_str(),"%x",&v_uintTmp);
                        rInt = v_uintTmp;
                    }
                    mType = 1;
                    mInt64Result= abs(rInt);
                    oss<<mInt64Result;
                    ret = oss.str();
                }else if(op == "FABS")
                {
                    mType = 2;
                    mdResult = fabs(rp);
                    sprintf(tmpData,"%.9f",mdResult);
                    ret = tmpData;
                }
                else if( op == "POW")/*指数和幂预算*/
                {
                    mType = 2;
                    mdResult = pow(lp,rp);
                    sprintf(tmpData,"%.9f",mdResult);
                    ret = tmpData;
                }else if( op == "|")
                {
                  std::stringstream sinl;
                  std::stringstream sinr;
                  unsigned int lInt = 0;
                  unsigned int rInt = 0;
                  sinl.clear();
                  sinl<<l;
                  if((!(sinl>>lInt))||isDigitHex(l))
                  {
                      scanf(l.c_str(),"%x",&v_uintTmp);
                      lInt = v_uintTmp;
                  }
                  sinr.clear();
                  sinr<<r;
                  if((!(sinr>>rInt))||isDigitHex(r))
                  {
                      scanf(r.c_str(),"%x",&v_uintTmp);
                      rInt = v_uintTmp;
                  }
                  mType = 1;
                  mInt64Result= (long long)(lInt|rInt);

                  oss<<mInt64Result;
                  ret = oss.str();
                }else if( op == "&")
                {
                    std::stringstream sinl;
                    std::stringstream sinr;
                    unsigned int lInt = 0;
                    unsigned int rInt = 0;
                    sinl.clear();
                    sinl<<l;
                    if((!(sinl>>lInt))||isDigitHex(l))
                    {
                        scanf(l.c_str(),"%x",&v_uintTmp);
                        lInt = v_uintTmp;
                    }
                    sinr.clear();
                    sinr<<r;
                    if((!(sinr>>rInt))||isDigitHex(r))
                    {
                        scanf(r.c_str(),"%x",&v_uintTmp);
                        rInt = v_uintTmp;
                    }
                    mType = 1;
                    mInt64Result= (long long)(lInt&rInt);

                    oss<<mInt64Result;
                    ret = oss.str();
                }else if( op == "~")
                {
                    std::stringstream sinl;
                    std::stringstream sinr;
                    unsigned int lInt = 0;
                    unsigned int rInt = 0;
                    sinl.clear();
                    sinl<<l;
                    if((!(sinl>>lInt))||isDigitHex(l))
                    {
                        scanf(l.c_str(),"%x",&v_uintTmp);
                        lInt = v_uintTmp;
                    }
                    sinr.clear();
                    sinr<<r;
                    if((!(sinr>>rInt))||isDigitHex(r))
                    {
                        scanf(r.c_str(),"%x",&v_uintTmp);
                        rInt = v_uintTmp;
                    }
                    mType = 1;
                    mInt64Result= (long long)(~rInt);

                    oss<<mInt64Result;
                    ret = oss.str();
                }
                else if( op == "^")
                {
                    std::stringstream sinl;
                    std::stringstream sinr;
                    unsigned int lInt = 0;
                    unsigned int rInt = 0;
                    sinl.clear();
                    sinl<<l;
                    if((!(sinl>>lInt))||isDigitHex(l))
                    {
                        scanf(l.c_str(),"%x",&v_uintTmp);
                        lInt = v_uintTmp;
                    }
                    sinr.clear();
                    sinr<<r;
                    if((!(sinr>>rInt))||isDigitHex(r))
                    {
                        scanf(r.c_str(),"%x",&v_uintTmp);
                        rInt = v_uintTmp;
                    }

                    mType = 1;
                    mInt64Result= lInt^rInt;
                    oss<<mInt64Result;
                    ret = oss.str();
                 }
                else if( op == "<<")
                {
                    std::stringstream sinl;
                    std::stringstream sinr;
                    unsigned int lInt = 0;
                    unsigned int rInt = 0;
                    sinl.clear();
                    sinl<<l;
                    if((!(sinl>>lInt))||isDigitHex(l))
                    {
                        scanf(l.c_str(),"%x",&v_uintTmp);
                        lInt = v_uintTmp;
                    }
                    sinr.clear();
                    sinr<<r;
                    if((!(sinr>>rInt))||isDigitHex(r))
                    {
                        scanf(r.c_str(),"%x",&v_uintTmp);
                        rInt = v_uintTmp;
                    }
                    mType = 1;
                    mInt64Result= lInt<<rInt;
                    oss<<mInt64Result;
                    ret = oss.str();
                }else if( op == ">>")
                {
                    std::stringstream sinl;
                    std::stringstream sinr;
                    unsigned int lInt = 0;
                    unsigned int rInt = 0;
                    sinl.clear();
                    sinl<<l;
                    if((!(sinl>>lInt))||isDigitHex(l))
                    {
                        scanf(l.c_str(),"%x",&v_uintTmp);
                        lInt = v_uintTmp;
                    }
                    sinr.clear();
                    sinr<<r;
                    if((!(sinr>>rInt))||isDigitHex(r))
                    {
                        scanf(r.c_str(),"%x",&v_uintTmp);
                        rInt = v_uintTmp;
                    }

                    mType = 1;
                    mInt64Result= lInt>>rInt;
                    oss<<mInt64Result;
                    ret = oss.str();
                }else if((op == "%")||(op == "MOD"))
                {
                    std::stringstream sinl;
                    std::stringstream sinr;
                    unsigned int lInt = 0;
                    unsigned int rInt = 0;
                    sinl.clear();
                    sinl<<l;
                    if((!(sinl>>lInt))||isDigitHex(l))
                    {
                        scanf(l.c_str(),"%x",&v_uintTmp);
                        lInt = v_uintTmp;
                    }
                    sinr.clear();
                    sinr<<r;
                    if((!(sinr>>rInt))||isDigitHex(r))
                    {
                        scanf(r.c_str(),"%x",&v_uintTmp);
                        rInt = v_uintTmp;
                    }

                    mType = 1;
                    mInt64Result= (long long)(lInt%rInt);
                    oss<<mInt64Result;
                    ret = oss.str();
                }else if(op == ">")
                {
                    int result = 0;
                    if(lp>rp)
                    {
                        result = 1;
                    }else{
                        result = 0;
                    }
                    mType = 0;
                    mInt64Result= result;
                    oss<<mInt64Result;
                    ret = oss.str();
                }else if(op == ">=")
                {
                    int result = 0;
                    if((lp>rp)||((fabs(lp-rp)<0.0000001)))
                    {
                        result = 1;
                    }else{
                        result = 0;
                    }
                    mType = 0;
                    mInt64Result= result;
                    oss<<mInt64Result;
                    ret = oss.str();
                }else if(op == "<")
                {
                    int result = 0;
                    if(lp<rp)
                    {
                        result = 1;
                    }else{
                        result = 0;
                    }
                    mType = 0;
                    mInt64Result= result;
                    oss<<mInt64Result;
                    ret = oss.str();
                }else if(op == "<=")
                {
                    int result = 0;
                    if((lp<rp)||((fabs(lp-rp)<0.0000001)))
                    {
                        result = 1;
                    }else{
                        result = 0;
                    }
                    mType = 0;
                    mInt64Result= result;
                    oss<<mInt64Result;
                    ret = oss.str();
                }
                else if(op == "==")
                {
                    int result = 0;
                    if((fabs(lp-rp)<0.0000001))
                    {
                        result = 1;
                    }else{
                        result = 0;
                    }
                    mType = 0;
                    mInt64Result= result;
                    oss<<mInt64Result;
                    ret = oss.str();
                }
                else if(op == "!=")
                {
                    int result = 0;
                    if((fabs(lp-rp)<0.0000001))
                    {
                        result = 0;
                    }else{
                        result = 1;
                    }
                    mType = 0;
                    mInt64Result= result;
                    oss<<mInt64Result;
                    ret = oss.str();
                }
                else if(op == "&&")
                {
                    if(lp&&rp)
                        mInt64Result = 1;
                    else
                        mInt64Result = 0;
                    mType = 0;
                    oss<<mInt64Result;
                    ret = oss.str();
                }
                else if(op =="!")
                {
                    if(!rp)
                        mInt64Result = 1;
                    else
                        mInt64Result = 0;
                    mType = 0;
                    oss<<mInt64Result;
                    ret = oss.str();
                }
                else if(op == "||")
                {
                    if(lp||rp)
                        mInt64Result = 1;
                    else
                        mInt64Result = 0;
                    mType = 0;
                    oss<<mInt64Result;
                    ret = oss.str();
                }
                else
                {
                    ret = "Error";
                }
            }

            return ret;
        }
        bool CalcFormula::expression(const std::string& exp)
        {
            bool ret = false;
            std::queue<std::string> spExp;
            std::queue<std::string> postExp;
            if(SplitExpress(exp,spExp))
            {
                mFormula = exp;
            }else
            {
                return false;
            }

            if( transform(spExp, postExp) )
            {
                mstrResult = Calculate(postExp);
                double dvalue = 0.0;
                long long ivalue = 0;
                std::stringstream sin;
                sin.clear();
                sin<<mstrResult;
                sin>>ivalue;
                sin.clear();
                sin<<mstrResult;
                sin>>dvalue;
                if(isInt(mstrResult))
                {
                    mType = 0;
                    mInt64Result= ivalue;
                }else{
                    mType = 1;
                    mdResult = dvalue;
                }
                ret = (mstrResult != "Error");
            }
            else
            {
                mstrResult = "Error";
            }
            return ret;
        }
        std::string CalcFormula::Replacestr(const std::string& orignStr,const std::string& oldStr,const std::string& newStr)
        {
            size_t pos = 0;
            std::string tempStr = orignStr;
            std::string::size_type newStrLen = newStr.length();
            std::string::size_type oldStrLen = oldStr.length();
            while(true)
            {
                pos = tempStr.find(oldStr,pos);
                if(pos == std::string::npos) break;
                tempStr.replace(pos,oldStrLen,newStr);
                pos += newStrLen;
            }
            return tempStr;
        }
    }
}
