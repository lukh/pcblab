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
        typedef vector<string> ExtensionOrderList;



    public:
        GerberHandler();
        ~GerberHandler();

        /// Opens a folder containing all gerber files, based on extensions
        void openFolder(const string &inFolderName, ExtensionOrderList inOrder);

        /// Opens a folder containing all gerber files, based on files names (filenames are used for names and identifiers)
        /// The first entry ([0]) of the list is the top
        void openFolderWithList(const string &inFolderName, vector<string> inFileNames);

        /// Opens a Gerber Layer,
        /// The name is given to the layer object,
        /// The Filename is the effective filename
        /// the identifier is the one related to the ExtensionOrderMap
        /// The position is the layer position
        void openGerberLayer(const string &inName, const string &inFilename, const string &inIdentifier, uint8_t inPosition = 0);


        /// clean existing layers
        void close();

        /// get the number of layers
        uint8_t getLayersCount() const { return mLayers.size(); }

        /// return the layer specified by inIdx.
        GerberLayer * getLayer(uint8_t inIdx) const;

        /// return the layer identifier specified by inIdx.
        string getLayerIdentifier(uint8_t inIdx) const;

        plRectangle getBoundingBox() const;



        /// get the existing order list
        const ExtensionOrderList& getOrderList() const { return mMap; }

        /// set a new order list
        /// if a identifier doesn't exist in the layers, it is ignored, all missing identifiers in the list are added to the end
        void setOrderList(const ExtensionOrderList &inList);


    public:
        static ExtensionOrderList createExtensionOrderMap_Altium(){
            ExtensionOrderList map;

            map.push_back("GBO"); //silkscreen
            map.push_back("GBP"); //paste
            map.push_back("GBS"); //solder
            map.push_back("GBL"); //layer
            map.push_back("GTL"); //layer
            map.push_back("GTS"); //solder
            map.push_back("GTP"); //paste
            map.push_back("GTO"); //silkscreen
            map.push_back("GKO"); //KeepOut

            return map;
        }


    private:
        /// defines the map of layer, with their identifier
        typedef map<string, GerberLayer *> LayerMap;



    private:
        ExtensionOrderList mMap;

        LayerMap mLayers;
};

#endif

