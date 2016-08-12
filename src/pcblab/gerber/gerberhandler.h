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

        /// defines the map of layer, with their positions (0 is bottom)
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
        static ExtensionOrderMap createExtensionOrderMap(){
            ExtensionOrderMap map;

            map["GBO"] = 0; //silkscreen
            map["GBP"] = 1; //paste
            map["GBS"] = 2; //solder
            map["GBL"] = 3; //layer
            map["GTL"] = 4; //layer
            map["GTS"] = 5; //solder
            map["GTP"] = 6; //paste
            map["GTO"] = 7; //silkscreen

            return map;
        }
        static const ExtensionOrderMap sDefaultExtOrderMap;


    private:
        LayerMap mLayers;
};

#endif

