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
        IGraphicObject(eType inType, Aperture *inAperture): mAperture(inAperture), mValid(false), mType(inType) {
            d_printf("%%% Creating GraphicObject", 4, 0, false);
        }
        virtual ~IGraphicObject() {
            d_printf("%%% Deleting GraphicObject", 4, 0, false);
        }

        eType getType() { return mType; }

        virtual Rectangle getBoundingBox() = 0;

    protected:
        Aperture *mAperture; //should it be DCode ?

        bool mValid;

    private:
        eType mType;
};


/// Interface to handles tracks (arcs and draw)
class IGraphicObjectTrack{
    public:
        IGraphicObjectTrack(Point inStartPoint, Point inEndPoint): mStartPoint(inStartPoint), mEndPoint(inEndPoint){}

        const Point& getStartPoint() {return mStartPoint; }
        const Point& getEndPoint() { return mEndPoint; }


    protected:
        Point mStartPoint;
        Point mEndPoint;
};


/// defines a draw:
/// parameters are
///     start point
///     stop point
class GraphicObjectDraw: public IGraphicObject, public IGraphicObjectTrack{
    public:
        GraphicObjectDraw(Point inStartPoint, Point inEndPoint, Aperture *inAperture):
            IGraphicObject(IGraphicObject::eTypeLine, inAperture), IGraphicObjectTrack(inStartPoint, inEndPoint){
                mValid = true;
                d_printf("Creating GraphicObjectDraw: start =(" + to_string(inStartPoint.mX) + ", " +to_string(inStartPoint.mY) + ") end =(" + to_string(inEndPoint.mX) + ", " +to_string(inEndPoint.mY) + ")", 4, 0, false);
        }

        virtual ~GraphicObjectDraw() {}


        virtual Rectangle getBoundingBox();
};


/// defines an arc
/// parameters
///     start, stop and center point
///     quadrant mode, interpolation mode (CCW or CW)
class GraphicObjectArc: public IGraphicObject, public IGraphicObjectTrack{
    public:
        GraphicObjectArc(Point inStartPoint, Point inEndPoint, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture);

        virtual ~GraphicObjectArc() {}

        virtual Rectangle getBoundingBox();

    private:
        Point mCenterOffset;

        GraphicState::eQuadrantMode mQuadrantMode;
        GraphicState::eInterpolationMode mInterpolationMode;
};


/// defines a flash with the current aperture at the given point
class GraphicObjectFlash: public IGraphicObject{
    public:
        GraphicObjectFlash(Point inPoint, Aperture *inAperture): IGraphicObject(IGraphicObject::eTypeFlash, inAperture), mPoint(inPoint) { mValid = true; }


        virtual Rectangle getBoundingBox();

    private:
        Point mPoint;
};



/// defines a region.
///  a region has several contours, and the aperture is given for its attributes
/// a region is constructed with several commands
class GraphicObjectRegion: public IGraphicObject{
    public:
        /// a Contour describes a part of the region.
        /// It is construct by a set of D01/D02 commands
        class Contour{
            public:
                enum eContoursConnection{
                    eNotConnected,
                    eTouching,
                    eOverlapping
                };

                Contour() {}
                ~Contour();

                /// Adds a linear segment
                void addSegment(Point inStart, Point inStop);

                /// Adds an arc segment
                void addSegment(Point inStart, Point inStop, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode);

                /// checks if the contour is closed
                bool isClosed();

                /// Implements the closing of the contour
                void close();

                /// checks if the Point is in the contour (contour must be closed)
                bool isInside(Point inPoint);

                /// checks if the Segment crosses the contour
                bool isCrossing(IGraphicObject *inObject);

                static bool isCrossing(GraphicObjectDraw *inDraw1, GraphicObjectDraw *inDraw2);
                static bool isCrossing(GraphicObjectArc *inArc, GraphicObjectDraw *inDraw);
                static bool isCrossing(GraphicObjectArc *inArc1, GraphicObjectArc *inArc2);

                /// checks the connection with another contour
                eContoursConnection getConnection(const Contour &inContour);



                static IGraphicObjectTrack* convert2Track(IGraphicObject *inObject);


            private:
                vector <IGraphicObject *> mSegments;
        };

    public:
        static bool isPoolCleaned();

        /// Creates a new contour in the "contour pool"
        static void openContour();

        /// adds a linear segment to the current contour
        static void addSegment(Point inStart, Point inStop);

        /// adds an arc segment to the current contour
        static void addSegment(Point inStart, Point inStop, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode);

        /// closes a contour (D02)
        static void closeContour();


        /// When a G37 cmd is received, contours are sorted and regions created regarding the position of the contours, and pool is cleaned
        /// inAperture can be used only for the attributes
        static vector<GraphicObjectRegion *> createRegionsFromContours(Aperture *inAperture);

        /// cleans the pool for the next run
        static void cleanContoursPool();

        /// flush the pool (delete objects), in case something went wrong
        static void flushContoursPool();

    protected:
        /// A pointer on the current contour, when the contour is built (between a D01 / D02)
        static Contour *sContour;

        /// The list of the contour created during a G36/37 pair
        static vector<Contour *> sContours;


    public:
        GraphicObjectRegion(Aperture *inAperture): IGraphicObject(IGraphicObject::eTypeRegion, inAperture) { mValid=false; }

        void addContour(Contour *inContour);

        virtual ~GraphicObjectRegion();


        virtual Rectangle getBoundingBox();


    private:
        /// the internal contours when the Region is valid
        vector<Contour *> mContours;



};

#endif



















