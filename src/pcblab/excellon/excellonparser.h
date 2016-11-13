#ifndef EXCELLONPARSER_H
#define EXCELLONPARSER_H

#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>

#include "../common.h"

#include "excellonstate.h"

using namespace std;

class ExcellonParser
{
    public:
        ExcellonParser();
        virtual ~ExcellonParser () {}

    protected:
        /// Parses the data stream and build the gerber layer.
        /// returns true if the file is completeand without error
        bool parse(istream &inStream);




        // --- used by the parser to build the layer ---

        // <<< --- Interface of ExcellonParser
        /// create a new tool
        virtual void addTool() = 0;

        /// create a new hole
        virtual void addHole() = 0;


        virtual void setSection(ExcellonState::eSection inSection) = 0;

        virtual void setUnit(ExcellonState::eUnit inUnit) = 0;

        virtual void setNumberFormat(ExcellonState::eNumberFormat inFormat) = 0;

        virtual void setCoordMode(ExcellonState::eCoordinatesMode inCoordMode) = 0;

        virtual void setCurrentPoint(plPoint inPoint) = 0;

        virtual void setOriginPoint(plPoint inPoint) = 0;
        // >>>


    private:
        /// returns the current line
        static string getLine(istream& inStream);

        /// eat the current line
        static void eatLine(istream& inStream);


        bool parseMCode(istream& inStream, bool &outEndOfProgram);
        bool parseGCode(istream& inStream);
        bool parseTCode(istream &inStream);
        bool parseCoordinates(istream &inStream);

        uint8_t getCommandCode(istream &inStream);
};

#endif // EXCELLONPARSER_H
