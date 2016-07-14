#include "cairogerberviewer.h"

CairoGerberViewer::CairoGerberViewer(): IGerberView()
{
    mContext = NULL;
    mSurface = NULL;
}

CairoGerberViewer::~CairoGerberViewer(){

}

void CairoGerberViewer::drawAll()
{

}

void CairoGerberViewer::drawLayer(const GerberLayer &inLayer)
{

}

uint32_t CairoGerberViewer::getWidth()
{
    if(mSurface == NULL){
        return 0;
    }

    return cairo_image_surface_get_width(mSurface);
}

uint32_t CairoGerberViewer::getHeight()
{
    if(mSurface == NULL){
        return 0;
    }

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
