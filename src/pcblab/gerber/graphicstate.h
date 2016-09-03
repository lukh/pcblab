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

          bool isValid() const {
              //The number of decimal places must be 4, 5 or 6. The number of integer places must be not more than 6
              return mDecimals <= 6 && mDecimals >= 4 && mIntegers <= 6;
          }
    };


    enum eUnit{
        eUnitInch,
        eUnitMm,
        eUnitUndefined
    };

    enum eQuadrantMode{
        eQuadrantSingle,
        eQuadrantMulti,
        eQuadrantUndefined
    };

    enum eInterpolationMode{
        eInterpolLinear,
        eInterpolCWCircular,
        eInterpolCCWCircular,
        eInterpolUndefined
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
    GraphicState(): GraphicState(CoordinateFormat(), eUnitMm) {}
    GraphicState (CoordinateFormat inCoordFormat, eUnit inUnit):
        mCoordFormat(inCoordFormat),
        mUnit(inUnit),
        mCurrentAperture(NULL),
        mQuadrantMode(eQuadrantUndefined),
        mInterpolationMode(eInterpolUndefined),
        mCurrentPoint(plPoint()),
        mSR(StepAndRepeat()),
        mLevelPolarity(ePolDark),
        mRegMode(eRegModeOff)
    {

    }

    virtual ~GraphicState () {}


    const CoordinateFormat &getCoordFormat () const { return mCoordFormat; }
    void setCoordFormat(CoordinateFormat inCoordFormat) { mCoordFormat = inCoordFormat; }

    eUnit getUnit () const { return mUnit; }
    void setUnit(eUnit inUnit) { mUnit = inUnit; }

    Aperture* getCurrentAperture () const { return mCurrentAperture; }
    void setCurrentAperture(Aperture* inCurrentAperture) { mCurrentAperture = inCurrentAperture; }

    eQuadrantMode getQuadrantMode () const { return mQuadrantMode; }
    void setQuadrantMode(eQuadrantMode inQuadrantMode) { mQuadrantMode = inQuadrantMode; }

    eInterpolationMode getInterpolationMode () const { return mInterpolationMode; }
    void setInterpolationMode(eInterpolationMode inInterpolationMode) { mInterpolationMode = inInterpolationMode; }

    const plPoint &getCurrentPoint () const { return mCurrentPoint; }
    void setCurrentPoint(plPoint inCurrentPoint) {
        mCurrentPoint.updateCoordinates(inCurrentPoint);
    }

    const StepAndRepeat &getSR () const { return mSR; }
    //void setSR(StepAndRepeat inSR) { mSR = inSR; }

    eLevelPolarity getLevelPolarity () const { return mLevelPolarity; }
    void setLevelPolarity(eLevelPolarity inLevelPolarity) { mLevelPolarity = inLevelPolarity; }

    eRegionMode getRegMode () const { return mRegMode; }
    void setRegMode(eRegionMode inRegMode) { mRegMode = inRegMode; }

  private:
        CoordinateFormat mCoordFormat;
        eUnit mUnit;
        Aperture *mCurrentAperture;
        eQuadrantMode mQuadrantMode;
        eInterpolationMode mInterpolationMode;
        plPoint mCurrentPoint;
        StepAndRepeat mSR;
        eLevelPolarity mLevelPolarity;
        eRegionMode mRegMode;
};

#endif
