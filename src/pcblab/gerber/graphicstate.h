/*******************************************************************************
* Module: GraphicState
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/08
*******************************************************************************/


#ifndef GRAPHICSTATE_H
#define GRAPHICSTATE_H

#include <stdint.h>
#include "../common.h"
#include "aperture/aperture.h"

/// GraphicState Class.
/// Defines the current graphic state
class GraphicState {
  public:

    // ----------------------------------  enums ----------------------------

    /// The CoordinateFormat class.
    /// The coordinate format specifies the number of integer and decimal places in a coordinate number
    class CoordinateFormat{
      public:
          CoordinateFormat(): mIntegers(2), mDecimals(4) {}
          CoordinateFormat(uint8_t inIntegers, uint8_t inDecimals): mIntegers(inIntegers), mDecimals(inDecimals) {}
          uint8_t mIntegers;
          uint8_t mDecimals;
    };


    enum eUnit{
        eUnitInch,
        eUnitMm
    };

    enum eQuadrantMode{
        eQuadrantSingle,
        eQuadrantMulti
    };

    enum eInterpolationMode{
        eInterpolLinear,
        eInterpolCWCircular,
        eInterpolCCWCircular
    };

    enum eLevelPolarity{
        ePolDark,
        ePolClear
    };

    enum eRegionMode{
        eRegModeOn,
        eRegModeOff
    };

    /// defines the current step and repeat
    class StepAndRepeat{
        public:
            StepAndRepeat(): mX(1), mY(1), mI(0.0), mJ(0.0) {}

            uint16_t mX;
            uint16_t mY;

            double mI;
            double mJ;
    };


    //----------------------- GraphicState ------------------------------------
    GraphicState(): mCoordFormat(CoordinateFormat()), mUnit(eUnitMm) {}
    GraphicState (CoordinateFormat inCoordFormat, eUnit inUnit): mCoordFormat(inCoordFormat), mUnit(inUnit) {}
    virtual ~GraphicState () {};


    CoordinateFormat &getCoordFormat () { return mCoordFormat; }
    void setCoordFormat(CoordinateFormat inCoordFormat) { mCoordFormat = inCoordFormat; }

    eUnit getUnit () { return mUnit; }
    void setUnit(eUnit inUnit) { mUnit = inUnit; }

    Aperture* getCurrentAperture () { return mCurrentAperture; }
    void setCurrentAperture(Aperture* inCurrentAperture) { mCurrentAperture = inCurrentAperture; }

    eQuadrantMode getQuadrantMode () { return mQuadrantMode; }
    void setQuadrantMode(eQuadrantMode inQuadrantMode) { mQuadrantMode = inQuadrantMode; }

    eInterpolationMode getInterpolationMode () { return mInterpolationMode; }
    void setInterpolationMode(eInterpolationMode inInterpolationMode) { mInterpolationMode = inInterpolationMode; }

    Point &getCurrentPoint () { return mCurrentPoint; }
    void setCurrentPoint(Point inCurrentPoint) { mCurrentPoint = inCurrentPoint; }

    StepAndRepeat & getSR () { return mSR; }
    //void setSR(StepAndRepeat inSR) { mSR = inSR; }

    eLevelPolarity getLevelPolarity () { return mLevelPolarity; }
    void setLevelPolarity(eLevelPolarity inLevelPolarity) { mLevelPolarity = inLevelPolarity; }

    eRegionMode getRegMode () { return mRegMode; }
    void setRegMode(eRegionMode inRegMode) { mRegMode = inRegMode; }

  private:
        CoordinateFormat mCoordFormat;
        eUnit mUnit;
        Aperture *mCurrentAperture;
        eQuadrantMode mQuadrantMode;
        eInterpolationMode mInterpolationMode;
        Point mCurrentPoint;
        StepAndRepeat mSR;
        eLevelPolarity mLevelPolarity;
        eRegionMode mRegMode;
};

#endif
