#include "common.h"

#include <iostream>

bool isNumber(char inChar){
    return ((inChar >= '0') && (inChar <= '9'));
}


uint8_t charToNum(char inChar){
    return (inChar - '0');
}


double stringToDouble(string &str){
    //std::replace(str.begin(), str.end(), '.', ',');
    return stod(str);
}


void d_printf(const string &str, int inLevel, int inTabs, bool inLeftAlign){
#ifdef DEBUG_PRINT
    /*if(inLevel > 1){
        return;
    }*/

    string tabs;

    if(inLeftAlign){
        for(int i=0; i < inTabs; i ++){
            tabs+= "\t";
        }
    }
    else{
        tabs += "\t\t\t\t\t\t\t\t\t";
    }

    /*string color;
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
    }*/


    cout << tabs << str << endl;

#endif

}

void err_printf(const string &str){
    cerr << "!!! [" <<str << "]" << endl;
}








bool isEqual(double a, double b){
    return fabs(a-b) <= kEqThreshold;
}



Rectangle::Rectangle(Point p1, Point p2)
{
    mPbl.mX = (p1.mX < p2.mX) ? p1.mX : p2.mX;
    mPbl.mY = (p1.mY < p2.mY) ? p1.mY : p2.mY;

    mPtr.mX = (p1.mX > p2.mX) ? p1.mX : p2.mX;
    mPtr.mY = (p1.mY > p2.mY) ? p1.mY : p2.mY;
}

Rectangle Rectangle::getBounds(Rectangle r1, Rectangle r2)
{
    double x1, x2, y1, y2;
    Point pbl, ptr;

    // Bottom Left
    x1 = r1.getBottomLeft().mX;
    x2 = r2.getBottomLeft().mX;
    pbl.mX =  x1 < x2 ? x1 : x2;

    y1 = r1.getBottomLeft().mY;
    y2 = r2.getBottomLeft().mY;
    pbl.mY =  y1 < y2 ? y1 : y2;

    //Top Right
    x1 = r1.getTopRight().mX;
    x2 = r2.getTopRight().mX;
    pbl.mX =  x1 > x2 ? x1 : x2;

    y1 = r1.getTopRight().mY;
    y2 = r2.getTopRight().mY;
    ptr.mY =  y1 > y2 ? y1 : y2;

    return Rectangle(pbl, ptr);
}

Rectangle Rectangle::getBounds(vector<Point> &inPoints)
{
    if (inPoints.size() == 0){
        return Rectangle();
    }

    Point p_min = inPoints.at(0);
    Point p_max = inPoints.at(0);

    for(vector<Point>::iterator it = inPoints.begin(); it != inPoints.end(); ++it){
        Point p = *it;

        p_min.mX = (p.mX < p_min.mX) ? p.mX : p_min.mX;
        p_min.mY = (p.mY < p_min.mY) ? p.mY : p_min.mY;
        p_max.mX = (p.mX > p_max.mX) ? p.mX : p_max.mX;
        p_max.mY = (p.mY > p_max.mY) ? p.mY : p_max.mY;
    }

    return Rectangle(p_min, p_max);
}

