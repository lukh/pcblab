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
    Point(): mX(0.0), mY(0.0), mIsXOmitted(false), mIsYOmitted(false) {}
    Point(double inX, double inY): mX(inX), mY(inY), mIsXOmitted(false), mIsYOmitted(false) {}


    /// updates the point regarding Is_Omitted, allowing to keep the previous value of one of the coordinates
    void updateCoordinates(Point &inPoint){
        //here, the point is modal (if one coordinate is ommitted, it uses the last one
        if(! inPoint.mIsXOmitted) {
            mX = inPoint.mX;
        }

        if(! inPoint.mIsYOmitted) {
            mY = inPoint.mY;
        }

        mIsXOmitted=false;
        mIsYOmitted=false;
    }

    double mX;
    double mY;

    bool mIsXOmitted;
    bool mIsYOmitted;

};



#endif // COMMON_H
