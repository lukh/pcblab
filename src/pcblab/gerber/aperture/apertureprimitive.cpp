#include <math.h>
#include "apertureprimitive.h"

IAperturePrimitive::IAperturePrimitive(IAperturePrimitive::eType inType):
    mType(inType)
{
#ifdef DEBUG_PRINT
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
#endif
}

plRectangle IAperturePrimitive::rotateBoundingBox(plRectangle inRect, double inAngle)
{
    plPoint p1 = inRect.getBottomLeft();
    plPoint p2 = inRect.getTopRight();

    //compute with  rotation.
    //the four points of the rectangle
    plPoint pa(p1.mX,p1.mY), pb(p1.mX, p2.mY), pc(p2.mX,p1.mY), pd(p2.mX,p2.mY);

    //With Rotation
    // in radian
    double rot = inAngle* M_PI / 180;
    double cosTheta = cos(rot);
    double sinTheta = sin(rot);


    // defines the for point of the rotated rectangle
    plPoint pa_p(pa.mX*cosTheta - pa.mY*sinTheta, pa.mX*sinTheta + pa.mY*cosTheta);
    plPoint pb_p(pb.mX*cosTheta - pb.mY*sinTheta, pb.mX*sinTheta + pb.mY*cosTheta);
    plPoint pc_p(pc.mX*cosTheta - pc.mY*sinTheta, pc.mX*sinTheta + pc.mY*cosTheta);
    plPoint pd_p(pd.mX*cosTheta - pd.mY*sinTheta, pd.mX*sinTheta + pd.mY*cosTheta);


    //find min and max for x,y
    vector<plPoint> pts_list;
    pts_list.push_back(pa_p);
    pts_list.push_back(pb_p);
    pts_list.push_back(pc_p);
    pts_list.push_back(pd_p);


    return plRectangle::getBounds(pts_list);
}



APrimCircle::APrimCircle(): IAperturePrimitive(eCircle) {}

bool APrimCircle::isValid() const
{
    // params count = 5: Exp, D, X, Y, rot
    return mModifiers.size() == 5 || mModifiers.size() == 4;
}

plRectangle APrimCircle::getBoundingBox()
{
    if(!isValid()) { return plRectangle(); }

    //Bounding box without rotation
    double x1 = getX()-getDiameter()/2;
    double y1 = getY()-getDiameter()/2;
    double x2 = getX()+getDiameter()/2;
    double y2 = getY()+getDiameter()/2;

    plPoint p1(x1,y1), p2(x2,y2);

    plRectangle bb(p1, p2);

    return rotateBoundingBox(bb, getRot());
}

APrimVectorLine::APrimVectorLine(): IAperturePrimitive(eVectorLine) {}

bool APrimVectorLine::isValid() const
{
    return mModifiers.size() == 7 && getWidth() >= 0;
}

plRectangle APrimVectorLine::getBoundingBox()
{
    if(!isValid()) { return plRectangle(); }

    plPoint p1,p2;

    plPoint ps(getStartX(), getStartY());
    plPoint pe(getEndX(),   getEndY());

    double angle = atan2(pe.mY-ps.mY, pe.mX-ps.mX);

    p1.mX = ps.mX - getWidth()*sin(angle)/2.0;
    p2.mX = pe.mX + getWidth()*sin(angle)/2.0;

    p1.mY = ps.mY - getWidth()*cos(angle)/2.0;
    p2.mY = pe.mY + getWidth()*cos(angle)/2.0;

    plRectangle bb(p1, p2);

    return rotateBoundingBox(bb, getRot());
}

APrimCenterLine::APrimCenterLine(): IAperturePrimitive(eCenterLine) {}

bool APrimCenterLine::isValid() const
{
    return mModifiers.size() == 6;
}

plRectangle APrimCenterLine::getBoundingBox()
{
    if(!isValid()) { return plRectangle(); }

    plPoint p1, p2;

    p1.mX = getX() - getWidth()/2.0;
    p1.mY = getY() - getHeight()/2.0;

    p2.mX = getX() + getWidth()/2.0;
    p2.mY = getY() + getHeight()/2.0;


    // the rectangle without rotation
    plRectangle bb(p1, p2);

    return rotateBoundingBox(bb, getRot());
}

APrimOutline::APrimOutline(): IAperturePrimitive(eOutLine) {}

bool APrimOutline::isValid() const
{
    // at least, one point...
    if (mModifiers.size() < 7){
        return false;
    }

    return mModifiers.size() == 5+2*mModifiers.at(1);
}

plRectangle APrimOutline::getBoundingBox()
{
    if(!isValid()) { return plRectangle(); }

    vector <plPoint> pts;

    uint16_t num_p = getSubSequentPointsCount();
    for(uint16_t p_idx = 0; p_idx < num_p; p_idx ++){
        pts.push_back(getPoint(p_idx));
    }

    plRectangle bb = plRectangle::getBounds(pts);

    return rotateBoundingBox(bb, getRot());
}

APrimPolygon::APrimPolygon(): IAperturePrimitive(ePolygon) {}

bool APrimPolygon::isValid() const
{
    return (mModifiers.size() == 6) && !(getRot() != 0.0 && (getX() != 0.0 || getY() != 0.0));
}

plRectangle APrimPolygon::getBoundingBox()
{
    if(!isValid()) { return plRectangle(); }

    //Bounding box without rotation
    double x1 = getX()-getDiameter()/2;
    double y1 = getY()-getDiameter()/2;
    double x2 = getX()+getDiameter()/2;
    double y2 = getY()+getDiameter()/2;

    plPoint p1(x1,y1), p2(x2,y2);

    plRectangle bb(p1, p2);

    return rotateBoundingBox(bb, getRot());
}

APrimMoire::APrimMoire(): IAperturePrimitive(eMoire) {}

bool APrimMoire::isValid() const
{
    return mModifiers.size() == 9 && !(getRot() != 0.0 && (getX() != 0.0 || getY() != 0.0));
}

plRectangle APrimMoire::getBoundingBox()
{
    if(!isValid()) { return plRectangle(); }

    double outer_dia = getOuterDiaOfOutRing() > getCrossHairLength() ? getOuterDiaOfOutRing() : getCrossHairLength();

    //Bounding box without rotation
    double x1 = getX()-outer_dia/2;
    double y1 = getY()-outer_dia/2;
    double x2 = getX()+outer_dia/2;
    double y2 = getY()+outer_dia/2;

    plPoint p1(x1,y1), p2(x2,y2);

    plRectangle bb(p1, p2);

    return rotateBoundingBox(bb, getRot());
}

APrimThermal::APrimThermal(): IAperturePrimitive(eThermal) {}

bool APrimThermal::isValid() const
{
    bool is_thermal_specific_valid =  \
        getOuterDia() > getInnerDia() && \
        getInnerDia() >= 0 && \
        getGapThickness() < (getOuterDia()/sqrt(2.0));


    return mModifiers.size() == 6 && !(getRot() != 0.0 && (getX() != 0.0 || getY() != 0.0)) and is_thermal_specific_valid;
}

plRectangle APrimThermal::getBoundingBox()
{
    if(!isValid()) { return plRectangle(); }

    //Bounding box without rotation
    double x1 = getX()-getOuterDia()/2;
    double y1 = getY()-getOuterDia()/2;
    double x2 = getX()+getOuterDia()/2;
    double y2 = getY()+getOuterDia()/2;

    plPoint p1(x1,y1), p2(x2,y2);

    plRectangle bb(p1, p2);

    return rotateBoundingBox(bb, getRot());
}


