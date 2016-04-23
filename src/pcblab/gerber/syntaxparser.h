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

#include "aperture/aperturemodifier.h"

using namespace std;

class SyntaxParser {
    public:
        SyntaxParser ();
        virtual ~SyntaxParser () {}

    protected:
        /// Parses the data stream and build the gerber layer.
        /// returns true if the file is completeand without error
        bool parse(istream &inStream);


        // --- Used by the parser to build the gerber layer ---

        virtual void setUnit(GraphicState::eUnit inUnit) = 0;
        virtual void setCoordinateFormat(GraphicState::CoordinateFormat inFormat) = 0;
        virtual void setQuadrantMode(GraphicState::eQuadrantMode inQuadrantMode) = 0;
        virtual void setInterpolationMode(GraphicState::eInterpolationMode inInterpolationMode) = 0;
        virtual void setRegionMode(GraphicState::eRegionMode inRegMode) = 0;

        virtual void setCurrentAperture(uint32_t inDCode) = 0;

        ///Defines a standard aperture
        virtual void addAperture(uint32_t inDCode, string inTemplateName) = 0;
        virtual void setApertureModifiers(uint32_t inDCode, vector<ApertureModifier> &inModifiers) = 0;

        virtual void defineApertureTemplate(/*  */) = 0;

        virtual void addNewLevel(GraphicState::eLevelPolarity inPolarity) = 0;


        /// interpolate operation, D01 code, with X, Y I, J. Not used coordinates are null
        virtual void interpolate(Point inPointXY, Point inPointIJ) = 0;

        /// move operation, D02
        virtual void move(Point inPointXY) = 0;

        /// flash operation, D03
        virtual void flash(Point inPointXY) = 0;

        virtual void handleComment(string &inStr) = 0;


        /// convert coordinate from raw to a real life coord (regarding coord format and unit)
        virtual double convertCoordinate(long inRaw) = 0;




    private:
        /// used by the parser
        enum eXCode{
            eXCodeNone,
            eXCodeFs,
            eXCodeMo,
            eXCodeAd,
            eXCodeAm,
            eXCodeSr,
            eXCodeLp,
            eXCodeTf,
            eXCodeTa,
            eXCodeTd
        };

        // ---------------- DCode ---------------
        /// parses D01, D02, D03, Dnn commands
        bool parseDCode(istream &inStream);


        // ---------------- GCode ---------------
        /// parses Gxx commands
        bool parseGCode(istream &inStream);

        // ---------------- XCode ---------------
        /// parses extended commands
        bool parseXCode(istream &inStream);

        /// parses FS command
        bool parseXCode_FS(istream &inStream);
        /// parses MO cmd
        bool parseXCode_MO(istream &inStream);
        /// parses LP cmd
        bool parseXCode_LP(istream &inStream);
        /// parses AD cmd
        bool parseXCode_AD(istream &inStream);

        /// extracts a AParameters in AD Cmd
        /// return if other params are available or not (* or X)
        /// takes outStatus which defines if the extraction was clean
        bool extractApertureModifiers(istream &inStream, vector<ApertureModifier> &outModifiers, bool &outStatus);

        /// parses AM cmd
        bool parseXCode_AM(istream &inStream);

        bool extractAM_Content(string & inContent);



        // ---------------- Tools  -------------
        /// resets omitted attributes
        void resetPointsAttributes(){
            //reset the point attributes
            mXY.mIsXOmitted=true;
            mXY.mIsYOmitted=true;
            mIJ.mIsXOmitted=true;
            mIJ.mIsYOmitted=true;
        }


        uint8_t getOpCode(istream &inStream);
        eXCode getXCode(char ch1, char ch2);
        uint32_t getRawCoord(istream &inStream);


    private:
        Point mXY;
        Point mIJ;

};


#endif
