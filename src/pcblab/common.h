#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <math.h>

#include "config.h"

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


static const double kEqThreshold = 0.00001;
bool isEqual(double a, double b);


class plPoint{

  public:
    plPoint(): mX(0.0), mY(0.0), mIsXOmitted(false), mIsYOmitted(false) {}
    plPoint(double inX, double inY): mX(inX), mY(inY), mIsXOmitted(false), mIsYOmitted(false) {}


    /// updates the point regarding Is_Omitted, allowing to keep the previous value of one of the coordinates
    void updateCoordinates(plPoint &inPoint){
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

    bool operator ==(const plPoint &p) const{
        return (mX == p.mX) && (mY == p.mY);
    }

    bool operator !=(const plPoint &p) const{
        return (mX != p.mX) || (mY != p.mY);
    }

};





class plRectangle{
    public:
        plRectangle() {}
        plRectangle(plPoint p1, plPoint p2);
        plRectangle(double x1, double y1, double x2, double y2);

        inline plPoint getTopRight() const { return mPtr; }
        inline plPoint getBottomLeft() const { return mPbl; }

        inline double getX1() const { return mPbl.mX; }
        inline double getY1() const { return mPbl.mY; }

        inline double getX2() const { return mPtr.mX; }
        inline double getY2() const { return mPtr.mY; }

        inline double getW() const { return mPtr.mX - mPbl.mX; }
        inline double getH() const { return mPtr.mY - mPbl.mY; }

        static plRectangle getBounds(plRectangle r1,plRectangle r2);
        static plRectangle getBounds(vector<plPoint> &inPoints);

    private:
        plPoint mPbl;
        plPoint mPtr;
};



class Color{
    public:
        Color():mR(0), mG(0), mB(0) {}
        Color(uint8_t inR,uint8_t inG,uint8_t inB): mR(inR), mG(inG), mB(inB) {}

        uint8_t mR;
        uint8_t mG;
        uint8_t mB;
};



bool isNumber(char inChar);


uint8_t charToNum(char inChar);

double stringToDouble(const string &str);
int stringToInt(const string &str);

void d_printf(const string &str, int inLevel=2, int inTabs=0, bool inLeftAlign=true);
void err_printf(const string &str);


#endif // COMMON_H
