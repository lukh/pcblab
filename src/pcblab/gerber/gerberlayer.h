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
class GerberLayer {
    public:

        /// GerberLevel is a container holding all the graphic object with the same polarity
        class GerberLevel{
            public:
                GerberLevel(GraphicState::eLevelPolarity inPolarity): mPolarity(inPolarity) {}

                /// Adds an existing object to the level.
                void addObject(IGraphicObject *inObject) { mObjects.push_back(inObject); }

            private:
                const GraphicState::eLevelPolarity mPolarity;
                vector <IGraphicObject *> mObjects;
        };


        GerberLayer ();
        virtual ~GerberLayer ();


        // --- I/O methods. ---

        /// open from the file given
        bool open(string &inFileName);

    private:
        /// Name of the Layer
        string mName;

        /// Store the current graphic state of the parsing.
        GraphicState mState;

        /// Store the syntax parser for this file.
        SyntaxParser mParser;

        /// Handles the different GraphicObjects in a GerberLevel container.
        /// the 0 level is the bottom one, it can be overriden by upper layers.
        vector<GerberLevel> mLevels;

        /// Aperture Dictionary.
        vector<Aperture> mApertures;

        /// Apertures Template Dict
        vector<ApertureTemplate *> mApertureTemplates;
};


#endif
