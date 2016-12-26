#include "caironetlistrenderer.h"


CairoNetlistRenderer::CairoNetlistRenderer()
{

}

CairoNetlistRenderer::~CairoNetlistRenderer()
{

}

void CairoNetlistRenderer::draw(const NetlistHandler &inNetlistHandler)
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
    initFont(20);



    const Netlist &netlist = inNetlistHandler.getNetlist();


    // draw the net
    for (vector<string>::iterator it = mActiveNets.begin(); it != mActiveNets.end(); ++it){
        string net_name = *it;
        if(netlist.find(net_name) != netlist.end()){
            const Net &net = netlist.at(net_name);
            drawNet(net);
        }
    }
}

void CairoNetlistRenderer::drawNet(const Net &inNet)
{
    const vector<NetEntry> &entries = inNet.getEntries();

    for(vector<NetEntry>::const_iterator it = entries.begin(); it != entries.end(); ++it){
        const NetEntry &entry = *it;
        plPoint pos = entry.getPosition();

        // radius
        double ux=5, uy=5;
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

        cairo_fill(mContext);
    }
}
