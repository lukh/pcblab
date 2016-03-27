/*******************************************************************************
* Module: GerberLayer
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/08
*******************************************************************************/

#ifndef GERBERLAYER_H
#define GERBERLAYER_H

#include <stdint.h>
#include <vector>
#include <string.h>

#include "../common.h"
#include "graphicstate.h"
#include "syntaxparser.h"
#include "graphicobject.h"
#include "aperture/aperture.h"

using namespace std;

/// Describes Gerber Layer, defines by its file
/// Parses the file and generate Graphic Object.
class GerberLayer: public SyntaxParser {
    public:

        /// GerberLevel is a container holding all the graphic object with the same polarity
        class GerberLevel{
            public:
                GerberLevel(GraphicState::eLevelPolarity inPolarity): mPolarity(inPolarity) {}
                ~GerberLevel();


                // --- MakeGraphicObjects ---
                void makeGraphicObjectDraw(Point inStart, Point inStop, Aperture *inAperture);
                void makeGraphicObjectArc(Point inStart, Point inStop, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture);
                void makeGraphicObjectFlash(Point inPoint, Aperture *inAperture);





            private:
                const GraphicState::eLevelPolarity mPolarity;
                vector <IGraphicObject *> mObjects;
        };


        GerberLayer(): SyntaxParser() {}
        virtual ~GerberLayer();

        // ------------------------------------------------------
        // ----------------- I/O methods. -----------------------
        // ------------------------------------------------------

        /// open from the file given
        bool open(string &inFileName);





    protected:
        // ------------------------------------------------------
        // ----- parser's virtual methods implementation --------
        // ------------------------------------------------------
        virtual void setUnit(GraphicState::eUnit inUnit){
            mState.setUnit(inUnit);
        }

        virtual void setCoordinateFormat(GraphicState::CoordinateFormat inFormat){
            mState.setCoordFormat(inFormat);
        }

        virtual void setQuadrantMode(GraphicState::eQuadrantMode inQuadrantMode){
            mState.setQuadrantMode(inQuadrantMode);
        }

        virtual void setInterpolationMode(GraphicState::eInterpolationMode inInterpolationMode){
            mState.setInterpolationMode(inInterpolationMode);
        }

        virtual void setCurrentPoint(Point inPoint){
            mState.setCurrentPoint(inPoint);
        }

        virtual void setRegionMode(GraphicState::eRegionMode inRegMode) {
            mState.setRegMode(inRegMode);
        };

        virtual void addNewLevel(GraphicState::eLevelPolarity inPolarity);

        virtual void defineAperture(/*  */);

        virtual void defineApertureTemplate(/*  */);

        virtual void setCurrentAperture(uint32_t inDCode);


        /// interpolate operation, D01 code, with X, Y I, J. Not used coordinates are null
        virtual void interpolate(Point inPointXY, Point inPointIJ);

        /// move operation, D02
        virtual void move(Point inPointXY);

        /// flash operation, D03
        virtual void flash(Point inPointXY);



    private:
        /// Name of the Layer
        string mName;

        /// Store the current graphic state of the parsing.
        GraphicState mState;

        /// Handles the different GraphicObjects in a GerberLevel container.
        /// the 0 level is the bottom one, it can be overriden by upper layers.
        vector<GerberLevel> mLevels;

        /// Store a pointer on the current level
        GerberLevel *mCurrentLevel;

        /// Aperture Dictionary.
        vector<Aperture> mApertures;

        /// Apertures Template Dict
        vector<ApertureTemplate *> mApertureTemplates;
};


#endif
