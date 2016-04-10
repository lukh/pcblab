#include "common.h"

#include <iostream>

bool isNumber(char inChar){
    return ((inChar >= '0') && (inChar <= '9'));
}


uint8_t charToNum(char inChar){
    return (inChar - '0');
}


double stringToDouble(string &str){
    std::replace(str.begin(), str.end(), '.', ',');
    return stod(str);
}


void d_printf(const string &str){
    cout << str << endl;
}

void err_printf(const string &str){
    cerr << "*** " << str << endl;
}
