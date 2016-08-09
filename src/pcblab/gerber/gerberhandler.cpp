#include "gerberhandler.h"

GerberHandler::GerberHandler()
{

}

GerberHandler::~GerberHandler()
{
    for(vector<GerberLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it){
        GerberLayer *layer = *it;
        delete layer;
    }
}

void GerberHandler::openFolder()
{

}

void GerberHandler::openGerberLayer(string &inName, string &inFilename, uint8_t inPosition)
{
    GerberLayer *layer = new GerberLayer(inName);

    layer->open(inFilename);

    mLayers.push_back(layer);
}
