#include "gerberhandler.h"

const GerberHandler::ExtensionOrderMap sDefaultExtOrderMap = GerberHandler::createExtensionOrderMap();

GerberHandler::GerberHandler()
{

}

GerberHandler::~GerberHandler()
{
    close();
}

void GerberHandler::openFolder(string &inFolderName, GerberHandler::ExtensionOrderMap &inOrder)
{

}


void GerberHandler::openGerberLayer(string &inName, string &inFilename, uint8_t inPosition)
{
    GerberLayer *layer = new GerberLayer(inName);

    layer->open(inFilename);

    mLayers[inPosition] = layer;
}




void GerberHandler::close()
{
    for(LayerMap::iterator it = mLayers.begin(); it != mLayers.end(); ++it){
        GerberLayer *layer = it->second;
        delete layer;
    }
}
