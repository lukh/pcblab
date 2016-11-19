#ifndef EXCELLONSTATE_H
#define EXCELLONSTATE_H

#include "pcblab/common.h"

class ExcellonState
{
    // enums
    public:
        enum eSection{
            eSectionHeader,
            eSectionBody,
            eSectionUndefined
        };

        enum eUnit{
            eUnitInch,
            eUnitMetric,
            eUnitUndefined
        };

        enum eNumberFormat{
            eNumForLeadingZeros,
            eNumForTrailingZeros,
            eNumForUndefined
        };

        enum eCoordinatesMode{
            eCoordModeAbsolute,
            eCoordModeIncremental
        };



    public:
        ExcellonState();



        eSection getSection() const;

        eUnit getUnit() const;

        eNumberFormat getNumberFormat() const;

        eCoordinatesMode getCoordMode() const;

        plPoint getCurrentPoint() const;

        plPoint getOriginPoint() const;

        uint8_t getCurrentToolIdx() const;


        void setSection(eSection inSection);

        void setUnit(eUnit inUnit);

        void setNumberFormat(eNumberFormat inFormat);

        void setCoordMode(eCoordinatesMode inCoordMode);

        void setCurrentPoint(plPoint inPoint);

        void setOriginPoint(plPoint inPoint);

        void setCurrentToolIdx(const uint8_t &currentToolIdx);

    private:
        eSection mSection;
        eUnit mUnit;
        eNumberFormat mNumberFormat;
        eCoordinatesMode mCoordMode;

        plPoint mCurrentPoint;
        plPoint mOriginPoint;

        uint8_t mCurrentToolIdx;
};

#endif // EXCELLONSTATE_H
