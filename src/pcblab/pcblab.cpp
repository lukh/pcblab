#include "pcblab.h"

PcbLab::PcbLab()
{

}

void PcbLab::openFolder(string &inFolderName)
{

}

void PcbLab::openGerberLayer(string &inName, string &inFilename, uint8_t inPosition)
{
    mGerber.openGerberLayer(inName, inFilename, inPosition);
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
