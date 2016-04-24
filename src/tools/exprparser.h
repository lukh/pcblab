#ifndef EXPRPARSER_H
#define EXPRPARSER_H



// testalgoexpr.cpp : Defines the entry podouble for the console application.
//

#include <string>
#include <map>
#include <iostream>
#include <sstream>

using namespace std;


class AlgorithmExpr {
    public:
        typedef map<string, double> SymbolTable;

        AlgorithmExpr(const string &inExpr) : mExpr(inExpr), mSymTable(NULL), mIdx(0) {

        }

        inline double evaluate(SymbolTable *inTable) {
            mSymTable = inTable;

            return expression();
        }

        inline double evaluate() {
            return expression();
        }

    private:
        char peek()
        {
            if (mExpr.size() <= mIdx)
                return '#';

            return mExpr.at(mIdx);
        }

        char get()
        {
            if (mExpr.size() <= mIdx)
                return '#';

            return mExpr.at(mIdx++);
        }

        double number()
        {
            double result = get() - '0';
            //integer part
            while (peek() >= '0' && peek() <= '9')
            {
                result = 10 * result + get() - '0';
            }

            //decimal
            if (peek() == '.') {
                get();
                double factor = 0.1;
                while (peek() >= '0' && peek() <= '9')
                {
                    result = result + (get() - '0')*factor;
                    factor *= 0.1;
                }
            }

            return result;
        }

        bool isFirstCharSymbol() {
            //the test is valid only for the first char !!!
            static string notSymbols("#+-/x()0123456789");

            bool ret = (notSymbols.find(peek()) == string::npos);

            return ret;
        }

        double symbol() {
            static string notSymbols("#+-/x()0123456789");
            static string endOfSymbol("#+-/x()");

            if (mSymTable == NULL || !isFirstCharSymbol()) {
                return 0;
            }

            //let's accumulate the symbol name...
            string sym;
            while (endOfSymbol.find(peek()) == string::npos) {
                sym.push_back(get());
            }

            if (mSymTable->count(sym) == 1) {
                return mSymTable->at(sym);
            }
            else {
                return 0;
            }
        }


        double factor()
        {
            if (peek() >= '0' && peek() <= '9') {
                return number();
            }
            else if (isFirstCharSymbol()) {
                return symbol();
            }

            else if (peek() == '(')
            {
                get(); // '('
                double result = expression();
                get(); // ')'
                return result;
            }
            else if (peek() == '-')
            {
                get();
                return -factor();
            }
            return 0; // error
        }

        double term()
        {
            double result = factor();
            while (peek() == 'x' || peek() == '/')
                if (get() == 'x')
                    result *= factor();
                else
                    result /= factor();
            return result;
        }

        double expression()
        {
            double result = term();
            while (peek() == '+' || peek() == '-')
                if (get() == '+')
                    result += term();
                else
                    result -= term();
            return result;
        }



        string mExpr;
        SymbolTable *mSymTable;
        uint32_t mIdx;
};



#endif // EXPRPARSER_H
