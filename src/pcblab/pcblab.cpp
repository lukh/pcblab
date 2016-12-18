#include "pcblab.h"

PcbLab::PcbLab()
{

}

void PcbLab::openFolder(const string &inFolderName, GerberHandler::ExtensionOrderList inOrder)
{
    mGerber.openFolder(inFolderName, inOrder);
    mExcellon.openFolder(inFolderName);
    mComponent.openFolder(inFolderName);
    mNetlist.openFolder(inFolderName);
}
