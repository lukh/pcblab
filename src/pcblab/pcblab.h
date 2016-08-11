#ifndef PCBLAB_H
#define PCBLAB_H

#include <string>
#include <iostream>
#include <fstream>

#include "gerber/gerberhandler.h"

using namespace std;

/// A manager for a full set of pcba object (gerber, nets, components)
/// Main object manipulated by the UI
class PcbLab
{
    public:
        PcbLab();


        /// Opens a folder containing all CAD files (gerbers, pnp, netlists, etc)
        void openFolder(string &inFolderName);


        /// Opens a specific layer
        void openGerberLayer(string &inName, string &inFilename, uint8_t inPosition);

        /// Opens a PickAndPlace layer
        void openPnPLayer(string &inFilename);

        /// Open a netlist
        void openNetlistLayer(string &inFilename);


        /// Closes the folder
        void closeFolder();


    private:
        GerberHandler mGerber;

};

#endif // PCBLAB_H
