#ifndef PNPPARSER_H
#define PNPPARSER_H


#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#include "../common.h"

#include "component.h"

class PnpParser
{
    public:
        PnpParser();
        virtual ~PnpParser() {}

        void setDesignatorString(string inDesString) { mDesString = inDesString; }
        void setXString(string inXStr) { mXString = inXStr; }
        void setYString(string inYStr) { mYString = inYStr; }
        void setRotString(string inRotStr) { mRotString = inRotStr; }

        /// Parses the data stream and build the layer.
        /// returns true if the file is complete and without error
        bool parse(istream &inStream, Components &outComponents);

    private:
        vector<string> mHeader;

        string mDesString;
        string mXString;
        string mYString;
        string mRotString;
};

#endif // PNPPARSER_H
