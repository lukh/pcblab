#include "icairorenderer.h"

void ICairoRenderer::setViewer(ICairoViewer &mViewer)
{
    mContext = mViewer.getContext();
}

void ICairoRenderer::initFont(double inFontSize)
{
    cairo_matrix_t ctm, font_matrix;

    deinitFont();
    if(mScaledFont != NULL || mFontOpts != NULL || mContext == NULL){
        return;
    }

    // line width
    double ux=inFontSize, uy=inFontSize;
    cairo_device_to_user_distance (mContext, &ux, &uy);
    if (ux < uy)
        ux = uy;



    cairo_select_font_face(mContext, "Sans",
          CAIRO_FONT_SLANT_NORMAL,
          CAIRO_FONT_WEIGHT_BOLD);
    cairo_font_face_t *font_face = cairo_get_font_face(mContext);

    mFontOpts = cairo_font_options_create();
    cairo_get_matrix( mContext, &ctm );
    cairo_get_font_matrix( mContext, &font_matrix );

    font_matrix.xx = ux;
    font_matrix.yy = -ux; // font size

    // font_face initialized elsewhere and is valid

    mScaledFont = cairo_scaled_font_create( font_face, &font_matrix, &ctm, mFontOpts );

    cairo_set_scaled_font(mContext, mScaledFont);

}

void ICairoRenderer::deinitFont()
{
    if(mScaledFont != NULL){
        cairo_scaled_font_destroy(mScaledFont);
        mScaledFont = NULL;
    }

    if(mFontOpts != NULL){
        cairo_font_options_destroy(mFontOpts);
        mFontOpts = NULL;
    }
}
