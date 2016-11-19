#include "cairoexcellonrenderer.h"

CairoExcellonRenderer::CairoExcellonRenderer(): IExcellonRenderer(),ICairoRenderer()
{

}

void CairoExcellonRenderer::draw(ExcellonHandler &inExcellonHandler)
{
    if(mContext == NULL){
        return;
    }

    // line width
    double ux=1, uy=1;
    cairo_device_to_user_distance (mContext, &ux, &uy);
    if (ux < uy)
        ux = uy;
    cairo_set_line_width (mContext, ux);


    vector <ExcellonHandler::Hole> &holes = inExcellonHandler.getHoles();


    for(vector <ExcellonHandler::Hole>::const_iterator it = holes.begin(); it != holes.end(); ++it){
        ExcellonHandler::Hole h = *it;

        double x, y, radius;
        x = h.getPoint().mX;
        y = h.getPoint().mY;
        radius = h.getTool().mDiameter/2;

        cairo_set_source_rgba(mContext, 0.0, 0.0, 0.0,1.0);

        cairo_move_to (mContext, x, x);
        cairo_arc(mContext, x, y, radius, 0, 2 * M_PI);

        cairo_fill(mContext);

        cairo_set_source_rgba(mContext, (double)mColor.mR/255.0, (double)mColor.mG/255.0, (double)mColor.mB/255.0, (double)mTransparency/255.0);

        cairo_move_to(mContext, x-radius, y);
        cairo_line_to(mContext, x+radius, y);
        cairo_move_to(mContext, x, y-radius);
        cairo_line_to(mContext, x, y+radius);

        cairo_stroke(mContext);
    }
}
