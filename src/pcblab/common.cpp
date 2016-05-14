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


void d_printf(const string &str, int inLevel, int inTabs, bool inLeftAlign){
    string tabs;

    /*if(inLevel > 1){
        return;
    }*/

    if(inLeftAlign){
        for(int i=0; i < inTabs; i ++){
            tabs+= "\t";
        }
    }
    else{
        tabs += "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
    }

    string color;
    if(inLevel <= 1){
        color = "orange";
    }
    else if(inLevel <= 2){
        color = "purple";
    }
    else if(inLevel <= 3){
        color = "blue";
    }
    else{
        color = "black";
    }

    cout << tabs + "<font color=" + color + ">" + str + "</font>" << endl;
}

void err_printf(const string &str){
    cerr << "<font color=red>*** " + str + "</font>" << endl;
}
