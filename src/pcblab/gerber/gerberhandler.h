#ifndef GERBERHANDLER_H
#define GERBERHANDLER_H

#include <stdint.h>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <list>

#include "tools/fio.h"

#include "gerberlayer.h"

using namespace std;

/// A class dedicated to contain all gerber layers
class GerberHandler{
    public:
        /// defines a map of extension for searching/opening a folder
        /// The index is the position, 0 is the top
        typedef list<string> ExtensionOrderList;



    public:
        GerberHandler();
        ~GerberHandler();

        /// Opens a folder containing all gerber files, based on extensions
        void openFolder(const string &inFolderName, ExtensionOrderList inOrder);

        /// Opens a folder containing all gerber files, based on files names (filenames are used for names and identifiers)
        /// The first entry ([0]) of the list is the top
        void openFolder(const string &inFolderName, vector<string> inFileNames);

        /// Opens a Gerber Layer,
        /// The name is given to the layer object,
        /// The Filename is the effective filename
        /// the identifier is the one related to the ExtensionOrderMap
        /// The position is the layer position
        void openGerberLayer(const string &inName, const string &inFilename, const string &inIdentifier, uint8_t inPosition = 0);


        /// clean existing layers
        void close();


    private:
        /// defines the map of layer, with their identifier
        typedef map<string, GerberLayer *> LayerMap;


        static ExtensionOrderList createExtensionOrderMap(){
            ExtensionOrderList map;

            /*map["GBO"] = 0; //silkscreen
            map["GBP"] = 1; //paste
            map["GBS"] = 2; //solder
            map["GBL"] = 3; //layer
            map["GTL"] = 4; //layer
            map["GTS"] = 5; //solder
            map["GTP"] = 6; //paste
            map["GTO"] = 7; //silkscreen*/

            return map;
        }


    private:
        ExtensionOrderList mMap;

        LayerMap mLayers;
};

#endif

