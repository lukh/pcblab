#include "cairocomponentrenderer.h"

CairoComponentRenderer::CairoComponentRenderer()
{

}

CairoComponentRenderer::~CairoComponentRenderer() {
    deinitFont();
}

void CairoComponentRenderer::draw(const ComponentHandler &inComponentHandler)
{
    if(mContext == NULL){
        return;
    }

    // line width
    double ux=2, uy=2;
    cairo_device_to_user_distance (mContext, &ux, &uy);
    if (ux < uy)
        ux = uy;
    cairo_set_line_width (mContext, ux);

    // color and transp
    cairo_set_source_rgba(mContext, (double)mColor.mR/255.0, (double)mColor.mG/255.0, (double)mColor.mB/255.0, (double)mTransparency/255.0);

    //font
    initFont();

    //start drawing
    const Components compos = inComponentHandler.getComponents();

    //draw only the given list
    if(! mAllComponents){
        for(vector<string>::const_iterator dit = mActiveComponents.begin(); dit != mActiveComponents.end(); ++dit){
            string des = *dit;

            Components::const_iterator cit = compos.find(des);
            if(cit != compos.end()){
                const Component &comp = cit->second;
                drawComponent(comp);
            }
        }
    }

    //draw all
    else{
        for(Components::const_iterator cit = compos.begin(); cit != compos.end(); ++cit){
            const Component &comp = cit->second;
            drawComponent(comp);
        }
    }
}

void CairoComponentRenderer::drawComponent(const Component &inCompo)
{
    string des = inCompo.getDesignator();
    plPoint pos = inCompo.getPosition();

    //size of text
    cairo_text_extents_t extents;
    cairo_text_extents(mContext, des.c_str(), &extents);

    double x = pos.mX - (extents.width/2 + extents.x_bearing);
    double y = pos.mY - (extents.height/2 + extents.y_bearing);

    cairo_move_to (mContext, x, y);
    cairo_show_text (mContext, des.c_str());

    if(!mAllComponents){
        // radius
        double ux=20, uy=20;
        cairo_device_to_user_distance (mContext, &ux, &uy);
        if (ux < uy){ ux = uy; }
        double radius = ux;

        //line width
        ux = 3; uy = 3;
        cairo_device_to_user_distance (mContext, &ux, &uy);
        if (ux < uy){ ux = uy; }
        double width = ux;
        cairo_set_line_width(mContext, width);

        // draw a cirle
        cairo_move_to(mContext, pos.mX+radius, pos.mY);
        cairo_arc(mContext, pos.mX, pos.mY, radius, 0, 2*M_PI);

        //draw a cross around
        ux = 100; uy = 100;
        cairo_device_to_user_distance (mContext, &ux, &uy);
        if (ux < uy){ ux = uy; }
        double cross = ux;

        cairo_move_to(mContext, pos.mX - cross, pos.mY);
        cairo_line_to(mContext, pos.mX - radius, pos.mY);
        cairo_move_to(mContext, pos.mX + cross, pos.mY);
        cairo_line_to(mContext, pos.mX + radius, pos.mY);

        cairo_move_to(mContext, pos.mX, pos.mY - cross);
        cairo_line_to(mContext, pos.mX, pos.mY - radius);
        cairo_move_to(mContext, pos.mX, pos.mY + cross);
        cairo_line_to(mContext, pos.mX, pos.mY + radius);

        cairo_stroke(mContext);
    }
}

