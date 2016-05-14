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
    return mModifiers.size() == 5;
}

APrimVectorLine::APrimVectorLine(): IAperturePrimitive(eVectorLine) {}

bool APrimVectorLine::isValid()
{
    return mModifiers.size() == 7;
}

APrimCenterLine::APrimCenterLine(): IAperturePrimitive(eCenterLine) {}

bool APrimCenterLine::isValid()
{
    return mModifiers.size() == 6;
}

APrimOutline::APrimOutline(): IAperturePrimitive(eOutLine) {}

bool APrimOutline::isValid()
{
    if (mModifiers.size() < 6){
        return false;
    }


    return mModifiers.size() > 5+2*mModifiers.at(1);
}

APrimPolygon::APrimPolygon(): IAperturePrimitive(ePolygon) {}

bool APrimPolygon::isValid()
{
    return mModifiers.size() == 6;
}

APrimMoire::APrimMoire(): IAperturePrimitive(eMoire) {}

bool APrimMoire::isValid()
{
    return mModifiers.size() == 9;
}

APrimThermal::APrimThermal(): IAperturePrimitive(eThermal) {}

bool APrimThermal::isValid()
{
    return mModifiers.size() == 6;
}


