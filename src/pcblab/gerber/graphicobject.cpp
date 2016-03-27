#include "graphicobject.h"


GraphicObjectArc::GraphicObjectArc(Point inStartPoint, Point inEndPoint, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture):
    IGraphicObject(IGraphicObject::eTypeArc, inStartPoint, inAperture),
    mEndPoint(inEndPoint), mCenterOffset(inCenterOffset),
    mQuadrantMode(inQuadrantMode), mInterpolationMode(inInterpolationMode)
{
    if(mInterpolationMode == GraphicState::eInterpolLinear){
        cerr << "ERROR (GerberObjectArc::GerberObjectArc): Can't instanciate an arc with an interpolation set to linear..." << endl;
        mValid = false;
    }

    else{
        mValid = true;
    }

}
