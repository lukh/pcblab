/*******************************************************************************
* Module: GraphicObject
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/10
*******************************************************************************/

#ifndef GRAPHICOBJECT_H
#define GRAPHICOBJECT_H


#include <stdint.h>
#include <iostream>

#include "../common.h"
#include "aperture/aperture.h"
#include "igerberview.h"
#include "graphicstate.h"

using namespace std;


/// Interface for GraphicObject.
/// Informations such as polarity and levels are stored in their containers
class IGraphicObject{
    public:
        enum eType{
            eTypeLine,
            eTypeArc,
            eTypeFlash,
            eTypeRegion,

            eTypeNone
        };


        //IGraphicObject(): mType(eTypeNone) {}
        IGraphicObject(eType inType, Point inStartPoint, Aperture *inAperture):  mStartPoint(inStartPoint), mAperture(inAperture), mValid(false), mType(inType) {}

        virtual ~IGraphicObject() {}

        virtual void draw(IGerberView *inView) = 0;

    protected:
        Point mStartPoint;
        Aperture *mAperture; //should it be DCode ?

        bool mValid;

    private:
        eType mType;
};


class GraphicObjectDraw: public IGraphicObject{
    public:
        GraphicObjectDraw(Point inStartPoint, Point inEndPoint, Aperture *inAperture):
            IGraphicObject(IGraphicObject::eTypeLine, inStartPoint, inAperture), mEndPoint(inEndPoint) { mValid = true; }

        virtual ~GraphicObjectDraw() {}

        virtual void draw(IGerberView *inView) {}

    private:
        Point mEndPoint;
};



class GraphicObjectArc: public IGraphicObject{
    public:
        GraphicObjectArc(Point inStartPoint, Point inEndPoint, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture);

        virtual ~GraphicObjectArc() {}

        virtual void draw(IGerberView *inView) {}

    private:
        Point mEndPoint;
        Point mCenterOffset;

        GraphicState::eQuadrantMode mQuadrantMode;
        GraphicState::eInterpolationMode mInterpolationMode;
};

class GraphicObjectFlash: public IGraphicObject{
    public:
        GraphicObjectFlash(Point inStartPoint, Aperture *inAperture): IGraphicObject(IGraphicObject::eTypeFlash, inStartPoint, inAperture) {}

        virtual void draw(IGerberView *inView) {}
};

class GraphicObjectRegion: public IGraphicObject{
    public:
        GraphicObjectRegion(Point inStartPoint, Aperture *inAperture): IGraphicObject(IGraphicObject::eTypeRegion, inStartPoint, inAperture) {}


        virtual void draw(IGerberView *inView) {}
};

#endif
