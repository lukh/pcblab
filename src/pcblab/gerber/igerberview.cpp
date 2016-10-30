#include "igerberview.h"

uint32_t IGerberView::GraphicSettings::sColorIdx = 0;

void IGerberView::setRenderTransformation(plPoint p1, plPoint p2) {
    plRectangle r(p1, p2);
    setRenderTransformation(r);
}

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

bool IGerberView::getGraphicSettings(string inLayerIdentifier, GraphicSettings &outGraphSett){
    if(mGraphicSettingsMap.find(inLayerIdentifier) == mGraphicSettingsMap.end()){
        return false;
    }

    outGraphSett = mGraphicSettingsMap[inLayerIdentifier];
    return true;
}
