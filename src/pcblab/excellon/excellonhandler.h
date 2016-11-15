#ifndef EXCELLONHANDLER_H
#define EXCELLONHANDLER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "tools/fio.h"

#include "excellonparser.h"
#include "excellonstate.h"

using namespace std;

class ExcellonHandler: ExcellonParser
{
    public:
        ExcellonHandler();
        virtual ~ExcellonHandler();

        /// search into a folder the Excellon file and call openFile
        bool openFolder(const string &inFolderName);

        bool openExcellonLayer(const string &inFileName);

        void close();


    protected:
        // <<< --- Implementation of ExcellonParser
        /// create a new tool
        virtual void addTool(uint8_t inIdx, double inHoleSize, double inInfeedRate, double inSpindleRate);

        /// create a new hole
        virtual void addHole(const string &inRawX, const string & inRawY);


        virtual void setSection(ExcellonState::eSection inSection);

        virtual void setUnit(ExcellonState::eUnit inUnit);

        virtual void setNumberFormat(ExcellonState::eNumberFormat inFormat);

        virtual void setCoordMode(ExcellonState::eCoordinatesMode inCoordMode);

        virtual void setCurrentPoint(plPoint inPoint);

        virtual void setOriginPoint(plPoint inPoint);

        virtual void setCurrentTool(uint8_t inToolIdx);
        // >>>


    private:
        ExcellonState mState;
};

#endif // EXCELLONHANDLER_H
