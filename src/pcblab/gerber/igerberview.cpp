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

bool IGerberView::getAlphaChannel(string inLayerIdentifier, uint8_t &outAlphaValue) {
    if(mGraphicSettingsMap.find(inLayerIdentifier) == mGraphicSettingsMap.end()){
        return false;
    }

    outAlphaValue = mGraphicSettingsMap[inLayerIdentifier].mTransparency;

    return true;
}

bool IGerberView::getColor(string inLayerIdentifier, Color &outColor) {
    if(mGraphicSettingsMap.find(inLayerIdentifier) == mGraphicSettingsMap.end()){
        return false;
    }

    outColor = mGraphicSettingsMap[inLayerIdentifier].mColor;

    return true;
}
