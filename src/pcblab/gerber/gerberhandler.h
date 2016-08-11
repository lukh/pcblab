#ifndef GERBERHANDLER_H
#define GERBERHANDLER_H

#include <stdint.h>

#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "gerberlayer.h"

using namespace std;

/// A class dedicated to contain all gerber layers
class GerberHandler{
    public:
        /// defines a map of extension<>idx for searching/opening a folder
        typedef map<string, uint8_t> ExtensionOrderMap;

        /// defines the map of layer, with their positions
        typedef map<uint8_t, GerberLayer *> LayerMap;

    public:
        GerberHandler();
        ~GerberHandler();

        /// Opens a folder containing all gerber files
        void openFolder(string &inFolderName, ExtensionOrderMap &inOrder);

        /// Opens a Gerber Layer.
        void openGerberLayer(string &inName, string &inFilename, uint8_t inPosition);


        /// clean existing layers
        void close();



    private:
        LayerMap mLayers;
};

#endif
