#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>
using namespace std;

enum eExtension{
    eGTL,
    eGBL,
    eGTO,
    eGBO,
    eGTS,
    eGBS,


    eBOM,
    ePNP,

    eDRAW,

    eExtCount
};

typedef map<eExtension, string> PcbFiles;




class Point{

  public:
    Point(): mX(0.0), mY(0.0) {}
    Point(double inX, double inY): mX(inX), mY(inY) {}


    double mX;
    double mY;

};



#endif // COMMON_H
