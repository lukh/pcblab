#ifndef BOMPARSER_H
#define BOMPARSER_H

#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#include "../common.h"

#include "component.h"

class BomParser
{
    public:
        BomParser();
        virtual ~BomParser() {}

        void setDesignatorString(string inDesString) { mDesString = inDesString; }

        /// Parses the data stream and build the excellon layer.
        /// returns true if the file is complete and without error
        bool parse(istream &inStream, Components &outComponents);

    private:
        vector<string> mHeader;

        string mDesString;
};

#endif // BOMPARSER_H
