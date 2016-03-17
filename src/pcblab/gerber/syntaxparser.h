/*******************************************************************************
* Module: SyntaxParser
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/13
*******************************************************************************/

#ifndef SYNTAXPARSER_H
#define SYNTAXPARSER_H

#include <stdint.h>
#include <string>
#include <iostream>

#include "../common.h"

#include "graphicstate.h"

class SyntaxParser {
    public:
        SyntaxParser ();
        virtual ~SyntaxParser ();

    protected:
        /// Parses the data stream and build the gerber layer.
        void parse(char inChar);


        // --- Used by the parser to build the gerber layer ---

        virtual void setUnit(GraphicState::eUnit inUnit) = 0;
        virtual void setCoordinateFormat(GraphicState::CoordinateFormat inFormat) = 0;
        virtual void setQuadrantMode(GraphicState::eQuadrantMode inQuadrantMode) = 0;
        virtual void setInterpolationMode(GraphicState::eInterpolationMode inInterpolationMode) = 0;

        virtual void defineAperture(/*  */) = 0;
        virtual void defineApertureTemplate(/*  */) = 0;

        virtual void addNewLevel(GraphicState::eLevelPolarity inPolarity) = 0;


        virtual void makeGraphicObjectDraw(Point inStart, Point inStop) = 0;
        virtual void makeGraphicObjectArc(Point inStart, Point inStop) = 0;
        virtual void makeGraphicObjectFlash() = 0;
        virtual void makeGraphicObjectRegion() = 0;



    private:
        void parseDCode(char *inText);
        void parseGCode(char *inText);


#endif
