#include "icairoviewer.h"

uint32_t ICairoViewer::getWidth() const
{
    if(!isViewerReady()){ return 0; }
    return cairo_image_surface_get_width(mSurface);
}

uint32_t ICairoViewer::getHeight() const
{
    if(!isViewerReady()){ return 0; }
    return cairo_image_surface_get_height(mSurface);
}

void ICairoViewer::deinitCairo()
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

void ICairoViewer::initCairo(uint32_t inW, uint32_t inH)
{
    deinitCairo();

    if(mSurface == NULL){
        mSurface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, inW, inH);
    }

    if(mContext == NULL){
        mContext = cairo_create (mSurface);
    }
}

plPoint ICairoViewer::getPointInSourceCoords(plPoint inImgCoord) const
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

void ICairoViewer::getVectorInSourceCoords(double *inDx, double *inDy) const
{
    if(mContext == NULL){
        return;
    }

    cairo_device_to_user_distance(mContext, inDx, inDy);
}

plPoint ICairoViewer::getPointInImgCoords(plPoint inSourceCoord) const
{
    if(mContext == NULL){
        return plPoint();
    }

    double x, y;
    x = inSourceCoord.mX;
    y = inSourceCoord.mY;
    cairo_user_to_device(mContext, &x, &y);

    return plPoint(x, y);
}

void ICairoViewer::getVectorInImgCoords(double *inDx, double *inDy) const
{
    if(mContext == NULL){
        return;
    }

    cairo_user_to_device_distance(mContext, inDx, inDy);
}


void ICairoViewer::setRenderArea(plPoint p1, plPoint p2) {
    plRectangle r(p1, p2);
    setRenderArea(r);
}

