#ifndef GERBERHANDLER_H
#define GERBERHANDLER_H

#include <stdint.h>

#include <string>
#include <fstream>
#include <vector>

#include "gerberlayer.h"

using namespace std;

/// A class dedicated to contain all gerber layers
class GerberHandler{
    public:
        GerberHandler();
        ~GerberHandler();

        void openFolder();

        void openGerberLayer(string &inName, string &inFilename, uint8_t inPosition);

    private:
        vector<GerberLayer *> mLayers;
};

#endif
