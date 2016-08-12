#include <math.h>
#include "apertureprimitive.h"

IAperturePrimitive::IAperturePrimitive(IAperturePrimitive::eType inType):
    mType(inType)
{
    string str = "%%% Creating IAperturePrimitive: ";

    switch (inType) {
        case eCircle:
            str += "Circle";
            break;

        case eVectorLine:
            str += "VectorLine";
            break;

        case eCenterLine:
            str += "CenterLine";
            break;

        case eOutLine:
            str += "Polygon";
            break;

        case ePolygon:
            str +="Polygon";
            break;

        case eMoire:
            str += "Moire";
            break;

        case eThermal:
            str += "Thermal";
            break;

        default:
            str += "INVALID";
            break;
    }

    d_printf(str, 4, 0, false);
}



APrimCircle::APrimCircle(): IAperturePrimitive(eCircle) {}

bool APrimCircle::isValid()
{
    // params count = 5: Exp, D, X, Y, rot
    return mModifiers.size() == 5 || mModifiers.size() == 4;
}

Rectangle APrimCircle::getBoundingBox()
{
    //Bounding box without rotation
    double x1 = getX()-getDiameter()/2;
    double y1 = getY()-getDiameter()/2;
    double x2 = getX()+getDiameter()/2;
    double y2 = getY()+getDiameter()/2;

    //the for points of the rectangle
    Point p1(x1,y1), p2(x1, y2), p3(x2,y1), p4(x2,y2);

    //With Rotation
    // in radian
    double angle = getRot()* M_PI / 180;
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);


    // defines the for point of the rotated rectangle
    Point p1_p(p1.mX*cosTheta - p1.mY*sinTheta, p1.mX*sinTheta + p1.mY*cosTheta);
    Point p2_p(p2.mX*cosTheta - p2.mY*sinTheta, p2.mX*sinTheta + p2.mY*cosTheta);
    Point p3_p(p3.mX*cosTheta - p3.mY*sinTheta, p3.mX*sinTheta + p3.mY*cosTheta);
    Point p4_p(p4.mX*cosTheta - p4.mY*sinTheta, p4.mX*sinTheta + p4.mY*cosTheta);


    //find min and max for x,y
    vector<Point> pts_list;
    pts_list.push_back(p1_p);
    pts_list.push_back(p2_p);
    pts_list.push_back(p3_p);
    pts_list.push_back(p4_p);


    return Rectangle::getBounds(pts_list);
}

APrimVectorLine::APrimVectorLine(): IAperturePrimitive(eVectorLine) {}

bool APrimVectorLine::isValid()
{
    return mModifiers.size() == 7;
}

Rectangle APrimVectorLine::getBoundingBox()
{
    Point p1,p2;

    Point ps(getStartX(), getStartY());
    Point pe(getEndX(),   getEndY());

    double angle = atan2(pe.mY-ps.mY, pe.mX-ps.mX);

    p1.mX = ps.mX - getWidth()*sin(angle)/2.0;
    p2.mX = ps.mX + getWidth()*sin(angle)/2.0;

    p1.mY = ps.mY - getWidth()*cos(angle)/2.0;
    p1.mY = ps.mY + getWidth()*cos(angle)/2.0;

    //compute with  rotation.
    //TOCHECK
    p1.mX = p1.mX * cos(M_PI*getRot()/180.0);
    p2.mX = p2.mX * cos(M_PI*getRot()/180.0);

    p1.mY = p1.mY * sin(M_PI*getRot()/180.0);
    p2.mY = p2.mY * sin(M_PI*getRot()/180.0);

    return Rectangle(p1,p2);
}

APrimCenterLine::APrimCenterLine(): IAperturePrimitive(eCenterLine) {}

bool APrimCenterLine::isValid()
{
    return mModifiers.size() == 6;
}

Rectangle APrimCenterLine::getBoundingBox()
{
    //TOCHECK
    double relw, relh;

    relw = fabs(cos(M_PI*getRot()/180.0)) * getWidth() + fabs(sin(M_PI*getRot()/180.0)) * getHeight();
    relh = fabs(sin(M_PI*getRot()/180.0)) * getWidth() + fabs(cos(M_PI*getRot()/180.0)) * getHeight();

    Point p1(cos(M_PI*getRot()/180.0)*getX()-relw/2, sin(M_PI*getRot()/180.0)*getY()-relh/2);
    Point p2(cos(M_PI*getRot()/180.0)*getX()+relw/2, sin(M_PI*getRot()/180.0)*getY()+relh/2);

    return Rectangle(p1, p2);
}

APrimOutline::APrimOutline(): IAperturePrimitive(eOutLine) {}

bool APrimOutline::isValid()
{
    if (mModifiers.size() < 6){
        return false;
    }


    return mModifiers.size() > 5+2*mModifiers.at(1);
}

Rectangle APrimOutline::getBoundingBox()
{
}

APrimPolygon::APrimPolygon(): IAperturePrimitive(ePolygon) {}

bool APrimPolygon::isValid()
{
    return mModifiers.size() == 6;
}

Rectangle APrimPolygon::getBoundingBox()
{

}

APrimMoire::APrimMoire(): IAperturePrimitive(eMoire) {}

bool APrimMoire::isValid()
{
    return mModifiers.size() == 9;
}

Rectangle APrimMoire::getBoundingBox()
{

}

APrimThermal::APrimThermal(): IAperturePrimitive(eThermal) {}

bool APrimThermal::isValid()
{
    return mModifiers.size() == 6;
}

Rectangle APrimThermal::getBoundingBox()
{

}


