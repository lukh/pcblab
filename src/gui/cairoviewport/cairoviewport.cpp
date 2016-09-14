#include "cairoviewport.h"

CairoViewport::CairoViewport(): ICairoViewer()
{

}

CairoViewport::CairoViewport(uint32_t inW, uint32_t inH, cairo_surface_t *inSource): ICairoViewer()
{
    initCairo(inW, inH);
    setSource(inSource);

    cairo_set_antialias(mContext, CAIRO_ANTIALIAS_BEST);
}

CairoViewport::~CairoViewport(){
}

void CairoViewport::setRenderTransformation(const plRectangle &inArea) { mRenderArea = inArea; }


plPoint CairoViewport::getPointInSourceCoordinates(plPoint inImgCoord) const
{
    double x, y;
    x = inImgCoord.mX;
    y = inImgCoord.mY;
    cairo_device_to_user(mContext, &x, &y);

    return plPoint(x, y);
}

void CairoViewport::getVectorInSourceCoordinates(double *inDx, double *inDy) const
{
    cairo_device_to_user_distance(mContext, inDx, inDy);
}

void CairoViewport::refresh()
{
    // set the transformation matrix
    applyTransformation();


    //clean...
    cairo_set_source_rgb (mContext, 0, 0, 0);
    cairo_paint (mContext);

    //draw the image
    cairo_pattern_t * pattern = cairo_pattern_create_for_surface(mSourceSurface);
    cairo_pattern_set_filter(pattern, CAIRO_FILTER_BILINEAR);
    cairo_set_source(mContext, pattern);
    cairo_rectangle(mContext, 0.0, 0.0, cairo_image_surface_get_width(mSourceSurface), cairo_image_surface_get_height(mSourceSurface));
    cairo_fill(mContext);
}


void CairoViewport::applyTransformation()
{
    //reset...
    cairo_identity_matrix (mContext);

    // scale factor
    double sx, sy;
    sx = getWidth() / mRenderArea.getW();
    sy = getHeight() / mRenderArea.getH();

    //if(mPropMode == eKeepProportion){
    if(sx < sy){ sy=sx; }
    else { sx=sy; }
    //}

    double tx, ty;
    tx = mRenderArea.getBottomLeft().mX * sx;
    ty = mRenderArea.getBottomLeft().mY * sy;


    cairo_translate (mContext, tx, ty);
    cairo_scale (mContext, sx, sy);
}
