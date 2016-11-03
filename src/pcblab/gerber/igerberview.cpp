#include "igerberview.h"

uint32_t IGerberView::GraphicSettings::sColorIdx = 0;


void IGerberView::setAlphaChannel(string inLayerIdentifier, uint8_t inAlphaValue) {
    if(mGraphicSettingsMap.find(inLayerIdentifier) == mGraphicSettingsMap.end()){
        return;
    }
    mGraphicSettingsMap[inLayerIdentifier].mTransparency = inAlphaValue;
}

void IGerberView::setColor(string inLayerIdentifier, Color inColor) {
    if(mGraphicSettingsMap.find(inLayerIdentifier) == mGraphicSettingsMap.end()){
        return;
    }
    mGraphicSettingsMap[inLayerIdentifier].mColor = inColor;
}

IGerberView::GraphicSettings IGerberView::getGraphicSettings(string inLayerIdentifier, bool &outStatus) const{
    if(mGraphicSettingsMap.find(inLayerIdentifier) == mGraphicSettingsMap.end()){
        outStatus = false;
        return GraphicSettings();
    }

    outStatus = true;

    GraphicSettingsMap::const_iterator cgsmit = mGraphicSettingsMap.find(inLayerIdentifier);
    return cgsmit->second;
}
