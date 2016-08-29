#include "cairogerberviewer.h"

#include <math.h>

CairoGerberViewer::CairoGerberViewer(): IGerberView()
{
    mContext = NULL;
    mSurface = NULL;
}

CairoGerberViewer::~CairoGerberViewer(){

}

void CairoGerberViewer::drawAll(const GerberHandler &inGerber)
{
    if(!isViewerReady()){ return; }

    //clean...
    cairo_set_source_rgb (mContext, 0, 0, 0);
    cairo_paint (mContext);

    mColorList.reset();

    //transform
    applyRenderTransformation();

    //draw...
    uint8_t layers_cnt = inGerber.getLayersCount();
    for(uint8_t idx = 0; idx < layers_cnt; idx++){
        const GerberLayer *layer = inGerber.getLayer(idx);
        if(layer != NULL){
            drawLayer(layer);
        }
    }
}

void CairoGerberViewer::drawLayer(const GerberLayer *inLayer)
{
    if(!isViewerReady()){ return; }

    //layer color
    const Color &color = mColorList.getCurrentColor();
    cairo_set_source_rgb(mContext, (double)color.mR/255.0, (double)color.mG/255.0, (double)color.mB/255.0);

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

    //increment the color for next layer
    mColorList.increment();

}





uint32_t CairoGerberViewer::getWidth() const
{
    if(!isViewerReady()){ return 0; }

    return cairo_image_surface_get_width(mSurface);
}

uint32_t CairoGerberViewer::getHeight() const
{
    if(!isViewerReady()){ return 0; }

    return cairo_image_surface_get_height(mSurface);
}

void CairoGerberViewer::deinitCairo()
{
    if(mContext != NULL){
        cairo_destroy(mContext);
    }
    if(mSurface != NULL){
        cairo_surface_destroy(mSurface);
    }

    mContext = NULL;
    mSurface = NULL;
}

void CairoGerberViewer::initCairo(uint32_t inW, uint32_t inH)
{
    deinitCairo();

    if(mSurface == NULL){
        mSurface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, inW, inH);
    }

    if(mContext == NULL){
        mContext = cairo_create (mSurface);
    }
}

void CairoGerberViewer::applyRenderTransformation()
{
    //reset...
    cairo_identity_matrix (mContext);

    // scale factor
    double sx, sy;
    sx = getWidth() / (mRealWorldArea.getTopRight().mX-mRealWorldArea.getBottomLeft().mX);
    sy = getHeight() / (mRealWorldArea.getTopRight().mY-mRealWorldArea.getBottomLeft().mY);

    if(mPropMode == eKeepProportion){
        if(sx < sy){ sy=sx; }
        else { sx=sy; }
    }

    double tx, ty;
    tx = mRealWorldArea.getBottomLeft().mX * sx;
    ty = mRealWorldArea.getBottomLeft().mY * sy;

    /* translate the draw area before drawing.  We must translate the whole
       drawing down an additional displayHeight to account for the negative
       y flip done later */
    cairo_translate (mContext, -tx, ty + getHeight());
    /* scale the drawing by the specified scale factor (inverting y since
        cairo y axis points down) */
    cairo_scale (mContext, sx, -sy);
}


void CairoGerberViewer::setLevelPolarity(GraphicState::eLevelPolarity inPol)
{
    if(!isViewerReady()){ return; }

    if(inPol == GraphicState::ePolDark){
        cairo_set_operator (mContext, CAIRO_OPERATOR_OVER);
    }
    else{
        cairo_set_operator (mContext, CAIRO_OPERATOR_CLEAR);
    }
}


