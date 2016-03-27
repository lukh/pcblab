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
#include <vector>

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
        IGraphicObject(eType inType, Aperture *inAperture): mAperture(inAperture), mValid(false), mType(inType) {}

        virtual ~IGraphicObject() {}

        virtual void draw(IGerberView *inView) = 0;

    protected:
        Aperture *mAperture; //should it be DCode ?

        bool mValid;

    private:
        eType mType;
};


/// defines a draw:
/// parameters are
///     start point
///     stop point
class GraphicObjectDraw: public IGraphicObject{
    public:
        GraphicObjectDraw(Point inStartPoint, Point inEndPoint, Aperture *inAperture):
            IGraphicObject(IGraphicObject::eTypeLine, inAperture), mStartPoint(inStartPoint), mEndPoint(inEndPoint) { mValid = true; }

        virtual ~GraphicObjectDraw() {}

        virtual void draw(IGerberView *inView) {}

    private:
        Point mStartPoint;
        Point mEndPoint;
};


/// defines an arc
/// parameters
///     start, stop and center point
///     quadrant mode, interpolation mode (CCW or CW)
class GraphicObjectArc: public IGraphicObject{
    public:
        GraphicObjectArc(Point inStartPoint, Point inEndPoint, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture);

        virtual ~GraphicObjectArc() {}

        virtual void draw(IGerberView *inView) {}

    private:
        Point mStartPoint;
        Point mEndPoint;
        Point mCenterOffset;

        GraphicState::eQuadrantMode mQuadrantMode;
        GraphicState::eInterpolationMode mInterpolationMode;
};


/// defines a flash with the current aperture at the given point
class GraphicObjectFlash: public IGraphicObject{
    public:
        GraphicObjectFlash(Point inStartPoint, Aperture *inAperture): IGraphicObject(IGraphicObject::eTypeFlash, inAperture), mStartPoint(inStartPoint) { mValid = true; }

        virtual void draw(IGerberView *inView) {}

    private:
        Point mStartPoint;
};



/// defines a region.
///  a region has several contours, and the aperture is given for its attributes
/// a region is constructed with several commands
class GraphicObjectRegion: public IGraphicObject{
    public:
        GraphicObjectRegion(Aperture *inAperture): IGraphicObject(IGraphicObject::eTypeRegion, inAperture) { mValid=false; }



        virtual void draw(IGerberView *inView) {}

    private:
        /// a Contour describes a part of the region.
        /// It is construct by a set of D01/D02 commands
        class Contour{
            public:
                Contour();
                ~Contour();

                void defineStartPoint(Point inStart);

                void addSegment(Point inStop);
                void addSegment(Point inStop, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode);

                void close(Point inStop);

            private:
                vector <IGraphicObject *> mSegments;
        };
};

#endif
