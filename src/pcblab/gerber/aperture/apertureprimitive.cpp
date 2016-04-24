#include "apertureprimitive.h"

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

