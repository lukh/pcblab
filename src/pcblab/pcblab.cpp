#include "pcblab.h"

PcbLab::PcbLab()
{

}

void PcbLab::openFolder(string &inFolderName, GerberHandler::ExtensionOrderList inOrder)
{
    mGerber.openFolder(inFolderName, inOrder);
    //mPnp.open...
    //mNetList.open...
    //mDrills.open...
}


void PcbLab::openPnPLayer(string &inFilename)
{

}

void PcbLab::openNetlistLayer(string &inFilename)
{

}

void PcbLab::closeFolder()
{

}
