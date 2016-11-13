#include "excellonstate.h"

ExcellonState::ExcellonState():
    mSection(ExcellonState::eSectionUndefined),
    mUnit(ExcellonState::eUnitUndefined),
    mNumberFormat(ExcellonState::eNumForUndefined),
    mCoordMode(ExcellonState::eCoordModeAbsolute)
{

}

ExcellonState::eSection ExcellonState::getSection() const
{
    return mSection;
}

ExcellonState::eUnit ExcellonState::getUnit() const
{
    return mUnit;
}

ExcellonState::eNumberFormat ExcellonState::getNumberFormat() const
{
    return mNumberFormat;
}

ExcellonState::eCoordinatesMode ExcellonState::getCoordMode() const
{
    return mCoordMode;
}

plPoint ExcellonState::getCurrentPoint() const
{
    return mCurrentPoint;
}

plPoint ExcellonState::getOriginPoint() const
{
    return mOriginPoint;
}

void ExcellonState::setSection(eSection inSection)
{
    mSection = inSection;
}

void ExcellonState::setUnit(eUnit inUnit)
{
    mUnit = inUnit;
}

void ExcellonState::setNumberFormat(eNumberFormat inFormat)
{
    mNumberFormat = inFormat;
}

void ExcellonState::setCoordMode(eCoordinatesMode inCoordMode)
{
    mCoordMode = inCoordMode;
}

void ExcellonState::setCurrentPoint(plPoint inPoint)
{
    mCurrentPoint = inPoint;
}

void ExcellonState::setOriginPoint(plPoint inPoint)
{
    mOriginPoint = inPoint;
}


