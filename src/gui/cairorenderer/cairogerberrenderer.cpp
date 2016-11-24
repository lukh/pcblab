#include "cairogerberrenderer.h"

#include <math.h>

CairoGerberRenderer::CairoGerberRenderer(): IGerberRenderer()
{

}

CairoGerberRenderer::~CairoGerberRenderer(){

}


void CairoGerberRenderer::drawAll(const GerberHandler &inGerber)
{
    if(mContext == NULL){ return; }

    //draw...
    uint8_t layers_cnt = inGerber.getLayersCount();
    for(uint8_t idx = 0; idx < layers_cnt; idx++){
        const GerberLayer *layer = inGerber.getLayer(idx);
        if(layer != NULL){
            drawLayer(inGerber.getLayerIdentifier(idx), layer);
        }
    }
}

void CairoGerberRenderer::drawLayer(string inIdentifier, const GerberLayer *inLayer)
{
    if(mContext == NULL){ return; }

    //layer color and transparency
    GraphicSettingsMap::const_iterator gsmi;
    gsmi = mGraphicSettingsMap.find(inIdentifier);
    if (gsmi == mGraphicSettingsMap.end()) {
        mGraphicSettingsMap[inIdentifier] = GraphicSettings();
        // select next color
        GraphicSettings::increment();
    }


    const Color &color = mGraphicSettingsMap[inIdentifier].mColor;
    uint8_t transparency = mGraphicSettingsMap[inIdentifier].mTransparency;



    //update context for a new layer
    cairo_push_group(mContext);

    //navigate through levels
    for (uint16_t idx = 0; idx < inLayer->getLevelsCount(); idx ++){
        GerberLayer::GerberLevel *level = inLayer->getLevel(idx);

        //change the level polarity
        setLevelPolarity(level->getPolarity());

        vector<IGraphicObject *> gos = level->getObjects();
        for(vector<IGraphicObject *>::iterator it_go = gos.begin(); it_go != gos.end(); ++it_go){
            IGraphicObject *object = *it_go;

            //defines if it is hilighted
            bool isHilighted = find(mHilightedObjects.begin(), mHilightedObjects.end(), object) != mHilightedObjects.end();

            //update the alpha
            if(isHilighted){
                cairo_set_source_rgba(mContext, (double)color.mR/255.0, (double)color.mG/255.0, (double)color.mB/255.0, 1.0);
            }
            else{
                cairo_set_source_rgba(mContext, (double)color.mR/255.0, (double)color.mG/255.0, (double)color.mB/255.0, (double)transparency/255.0);
            }

            switch(object->getType()){
                case IGraphicObject::eTypeLine:{
                    GraphicObjectDraw *draw = static_cast<GraphicObjectDraw *>(object);
                    drawDraw(draw);
                    break;}
                case IGraphicObject::eTypeArc:{
                    GraphicObjectArc *arc = static_cast<GraphicObjectArc *>(object);
                    drawArc(arc);
                    break;}
                case IGraphicObject::eTypeFlash:{
                    GraphicObjectFlash *flash = static_cast<GraphicObjectFlash *>(object);
                    drawFlash(flash);
                    break;}
                case IGraphicObject::eTypeRegion:{
                    GraphicObjectRegion *region = static_cast<GraphicObjectRegion *>(object);
                    drawRegion(region);
                    break;}
                default:
                    break;
            }
        }
    }


    //update context for a layer
    cairo_pop_group_to_source(mContext);
    cairo_paint(mContext);
}





void CairoGerberRenderer::setLevelPolarity(GraphicState::eLevelPolarity inPol)
{
    if(mContext == NULL){ return; }

    if(inPol == GraphicState::ePolDark){
        cairo_set_operator (mContext, CAIRO_OPERATOR_OVER);
    }
    else{
        cairo_set_operator (mContext, CAIRO_OPERATOR_CLEAR);
    }
}


