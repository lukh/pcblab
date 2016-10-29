#include "cairogerberrenderer.h"

#include <math.h>

CairoGerberRenderer::CairoGerberRenderer(): IGerberView()
{

}

CairoGerberRenderer::~CairoGerberRenderer(){

}

void CairoGerberRenderer::drawAll(const GerberHandler &inGerber)
{
    if(!isViewerReady()){ return; }

    cairo_set_antialias(mContext, CAIRO_ANTIALIAS_BEST);

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

void CairoGerberRenderer::drawLayer(const GerberLayer *inLayer)
{
    if(!isViewerReady()){ return; }

    //layer color and transparency
    const Color &color = mColorList.getCurrentColor();

    TransparencyMap::const_iterator tmit;
    tmit = mTransparencyMap.find(inLayer->getName());
    if (tmit == mTransparencyMap.end()) { mTransparencyMap[inLayer->getName()] = 255; }

    uint8_t transparency = mTransparencyMap[inLayer->getName()];

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

    //increment the color for next layer
    mColorList.increment();

}



plPoint CairoGerberRenderer::getPointInRealWorldCoordinates(plPoint inImgCoord) const
{
    if(mContext == NULL){
        return plPoint();
    }

    double x, y;
    x = inImgCoord.mX;
    y = inImgCoord.mY;
    cairo_device_to_user(mContext, &x, &y);

    return plPoint(x, y);
}

void CairoGerberRenderer::getVectorInRealWorldCoordinates(double *inDx, double *inDy) const
{
    if(mContext == NULL){
        return;
    }

    cairo_device_to_user_distance(mContext, inDx, inDy);
}


void CairoGerberRenderer::applyRenderTransformation()
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


void CairoGerberRenderer::setLevelPolarity(GraphicState::eLevelPolarity inPol)
{
    if(!isViewerReady()){ return; }

    if(inPol == GraphicState::ePolDark){
        cairo_set_operator (mContext, CAIRO_OPERATOR_OVER);
    }
    else{
        cairo_set_operator (mContext, CAIRO_OPERATOR_CLEAR);
    }
}


