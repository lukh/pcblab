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
