#include "pcblab.h"

PcbLab::PcbLab()
{

}

void PcbLab::openFolder(const string &inFolderName, GerberHandler::ExtensionOrderList inOrder)
{
    mGerber.openFolder(inFolderName, inOrder);
    //mPnp.open...
    //mNetList.open...
    //mDrills.open...
}


void PcbLab::openPnPLayer(const string &inFilename)
{

}

void PcbLab::openNetlistLayer(const string &inFilename)
{

}

void PcbLab::closeFolder()
{

}
