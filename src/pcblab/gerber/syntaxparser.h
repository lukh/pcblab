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
        virtual ~SyntaxParser () {};

    protected:
        /// Parses the data stream and build the gerber layer.
        void parse(char inChar);


        // --- Used by the parser to build the gerber layer ---

        virtual void setUnit(GraphicState::eUnit inUnit) = 0;
        virtual void setCoordinateFormat(GraphicState::CoordinateFormat inFormat) = 0;
        virtual void setQuadrantMode(GraphicState::eQuadrantMode inQuadrantMode) = 0;
        virtual void setInterpolationMode(GraphicState::eInterpolationMode inInterpolationMode) = 0;
        virtual void setRegionMode(GraphicState::eRegionMode inRegMode) = 0;

        virtual void setCurrentAperture(uint32_t inDCode) = 0;

        virtual void defineAperture(/*  */) = 0;
        virtual void defineApertureTemplate(/*  */) = 0;

        virtual void addNewLevel(GraphicState::eLevelPolarity inPolarity) = 0;


        /// interpolate operation, D01 code, with X, Y I, J. Not used coordinates are null
        virtual void interpolate(Point inPointXY, Point inPointIJ) = 0;

        /// move operation, D02
        virtual void move(Point inPointXY) = 0;

        /// flash operation, D03
        virtual void flash(Point inPointXY) = 0;




    private:
        void parseDCode(char *inText);
        void parseGCode(char *inText);

};


#endif
