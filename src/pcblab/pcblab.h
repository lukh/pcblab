#ifndef PCBLAB_H
#define PCBLAB_H

#include <string>
#include <iostream>
#include <fstream>

#include "gerber/gerberhandler.h"
#include "excellon/excellonhandler.h"
#include "bom/componenthandler.h"
#include "netlist/netlisthandler.h"

using namespace std;

/// A manager for a full set of pcba object (gerber, nets, components)
/// Main object manipulated by the UI
class PcbLab
{
    public:
        PcbLab();


        /// Opens a folder containing all CAD files (gerbers, pnp, netlists, etc)
        void openFolder(const string &inFolderName, GerberHandler::ExtensionOrderList inOrder);


        const GerberHandler &getGerber() const { return mGerber; }
        ExcellonHandler &getExcellon() { return mExcellon; }
        ComponentHandler &getComponents() { return mComponent; }
        NetlistHandler &getNetlist() { return mNetlist; }


    private:
        GerberHandler mGerber;
        ExcellonHandler mExcellon;
        ComponentHandler mComponent;
        NetlistHandler mNetlist;

};

#endif // PCBLAB_H
