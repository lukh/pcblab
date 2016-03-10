/*******************************************************************************
* Module: GraphicState
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/08
*******************************************************************************/


##ifndef GRAPHICSTATE_H
#define GRAPHICSTATE_H

/// GraphicState Class.
/// Defines the current graphic state
class GraphicState {
  public:

    /// The CoordinateFormat class.
    /// The coordinate format specifies the number of integer and decimal places in a coordinate number
    class CoordinateFormat{
      public:
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
      uint16_t mX;
      uint16_t mY;

      double mI;
      double mJ;
    };


    GraphicState ();
    virtual ~GraphicState ();


  private:
    /* data */
};

#endif
