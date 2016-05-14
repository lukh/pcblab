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
    return true;
}

APrimVectorLine::APrimVectorLine(): IAperturePrimitive(eVectorLine) {}

bool APrimVectorLine::isValid()
{
    return true;
}

APrimCenterLine::APrimCenterLine(): IAperturePrimitive(eCenterLine) {}

bool APrimCenterLine::isValid()
{
    return true;
}

APrimOutline::APrimOutline(): IAperturePrimitive(eOutLine) {}

bool APrimOutline::isValid()
{
    return true;
}

APrimPolygon::APrimPolygon(): IAperturePrimitive(ePolygon) {}

bool APrimPolygon::isValid()
{
    return true;
}

APrimMoire::APrimMoire(): IAperturePrimitive(eMoire) {}

bool APrimMoire::isValid()
{
    return true;
}

APrimThermal::APrimThermal(): IAperturePrimitive(eThermal) {}

bool APrimThermal::isValid()
{
    return true;
}


