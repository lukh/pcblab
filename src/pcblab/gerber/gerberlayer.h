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


                // --- MakeGraphicObjects ---
                void makeGraphicObjectDraw(Point inStart, Point inStop, Aperture *inAperture);
                void makeGraphicObjectArc(Point inStart, Point inStop, Aperture *inAperture);
                void makeGraphicObjectFlash(Aperture *inAperture);
                void makeGraphicObjectRegion(Aperture *inAperture);




            private:
                const GraphicState::eLevelPolarity mPolarity;
                vector <IGraphicObject *> mObjects;
        };


        GerberLayer (): SyntaxParser() {}
        virtual ~GerberLayer ();

        // ------------------------------------------------------
        // ----------------- I/O methods. -----------------------
        // ------------------------------------------------------

        /// open from the file given
        bool open(string &inFileName);





    protected:
        // ------------------------------------------------------
        // ----- parser's virtual methods implementation --------
        // ------------------------------------------------------
        virtual void setUnit(GraphicState::eUnit inUnit);
        virtual void setCoordinateFormat(GraphicState::CoordinateFormat inFormat);
        virtual void setQuadrantMode(GraphicState::eQuadrantMode inQuadrantMode);
        virtual void setInterpolationMode(GraphicState::eInterpolationMode inInterpolationMode);

        virtual void defineAperture(/*  */);
        virtual void defineApertureTemplate(/*  */);

        virtual void addNewLevel(GraphicState::eLevelPolarity inPolarity);


        virtual void makeGraphicObjectDraw(Point inStart, Point inStop);
        virtual void makeGraphicObjectArc(Point inStart, Point inStop);
        virtual void makeGraphicObjectFlash();
        virtual void makeGraphicObjectRegion();



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
