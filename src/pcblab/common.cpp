#include "common.h"

#include <iostream>

bool isNumber(char inChar){
    return ((inChar >= '0') && (inChar <= '9'));
}


uint8_t charToNum(char inChar){
    return (inChar - '0');
}

void d_printf(const string &str){
    cout << str << endl;
}

void err_printf(const string &str){
    cerr << "*** " << str << endl;
}
