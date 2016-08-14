#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <math.h>

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

    bool operator ==(const Point &p) const{
        return (mX == p.mX) && (mY == p.mY);
    }

    bool operator !=(const Point &p) const{
        return (mX != p.mX) || (mY != p.mY);
    }

};





class Rectangle{
    public:
        Rectangle() {}
        Rectangle(Point p1, Point p2);

        inline Point getTopRight() { return mPtr; }
        inline Point getBottomLeft() { return mPbl; }

        static Rectangle getBounds(Rectangle r1,Rectangle r2);
        static Rectangle getBounds(vector<Point> &inPoints);

    private:
        Point mPbl;
        Point mPtr;
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

double stringToDouble(string &str);

void d_printf(const string &str, int inLevel=2, int inTabs=0, bool inLeftAlign=true);
void err_printf(const string &str);


#endif // COMMON_H
