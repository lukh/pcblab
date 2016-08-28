#include "cairogerberviewer.h"

#include <math.h>



void CairoGerberViewer::drawDraw(GraphicObjectDraw *inDraw)
{
    Point p0 = inDraw->getStartPoint();
    Point p1 = inDraw->getEndPoint();

    if(!isViewerReady()){ return; }

    if(inDraw == NULL){ return; }

    if(!inDraw->isValid()){
        err_printf("ERROR(CairoGerberViewer::drawDraw): The draw is invalid");
        return;
    }


    Aperture *ap = inDraw->getAperture();
    if(! ap->isValid() || ap->getPrimitives().size() < 1){
        err_printf("ERROR(CairoGerberViewer::drawDraw): The aperture is invalid");
        return;
    }

    // -- settings, line width, caps, etc
    const IApertureTemplate & at = ap->getTemplate();
    IAperturePrimitive *ip = ap->getPrimitives()[0];

    if(at.getName() == "C" && ip->getType() == IAperturePrimitive::eCircle){
        double width = static_cast<APrimCircle *>(ip)->getDiameter();

        cairo_set_line_cap(mContext, CAIRO_LINE_CAP_ROUND);
        cairo_set_line_join(mContext, CAIRO_LINE_JOIN_ROUND);

        //set the line
        double ux=width, uy=width;
        cairo_device_to_user_distance (mContext, &ux, &uy);
        if (ux < uy)
            ux = uy;
        cairo_set_line_width (mContext, ux);

        // draw the line
        cairo_move_to (mContext, r2p_X(p0.mX), r2p_Y(p0.mY));
        cairo_line_to (mContext, r2p_X(p1.mX), r2p_Y(p1.mY));
        cairo_stroke (mContext);
    }

    else if(at.getName() == "R" && ip->getType() == IAperturePrimitive::eCenterLine){
        double wp = static_cast<APrimCenterLine *>(ip)->getWidth();
        double hp = static_cast<APrimCenterLine *>(ip)->getHeight();


        //draw shape
        if(p0.mX < p1.mX && p0.mY < p1.mY){
            cairo_move_to (mContext, r2p_X(p0.mX - wp), r2p_Y(p0.mY - hp));
            cairo_line_to(mContext, r2p_X(p0.mX + wp), r2p_Y(p0.mY - hp));

            cairo_line_to(mContext, r2p_X(p1.mX + wp), r2p_Y(p1.mY - hp));
            cairo_line_to(mContext, r2p_X(p1.mX + wp), r2p_Y(p1.mY + hp));
            cairo_line_to(mContext, r2p_X(p1.mX - wp), r2p_Y(p1.mY + hp));

            cairo_line_to(mContext, r2p_X(p0.mX - wp), r2p_Y(p0.mY + hp));
            cairo_line_to(mContext, r2p_X(p0.mX - wp), r2p_Y(p0.mY - hp));
        }
        else if(p0.mX < p1.mX && p0.mY >= p1.mY){
            cairo_move_to (mContext, r2p_X(p0.mX - wp), r2p_Y(p0.mY + hp));
            cairo_line_to (mContext, r2p_X(p0.mX - wp), r2p_Y(p0.mY - hp));

            cairo_line_to (mContext, r2p_X(p1.mX - wp), r2p_Y(p1.mY - hp));
            cairo_line_to (mContext, r2p_X(p1.mX + wp), r2p_Y(p1.mY - hp));
            cairo_line_to (mContext, r2p_X(p1.mX + wp), r2p_Y(p1.mY + hp));

            cairo_line_to (mContext, r2p_X(p0.mX + wp), r2p_Y(p0.mY + hp));
            cairo_line_to (mContext, r2p_X(p0.mX - wp), r2p_Y(p0.mY + hp));
        }

        else if(p0.mX >= p1.mX && p0.mY < p1.mY){
            cairo_move_to (mContext, r2p_X(p0.mX + wp), r2p_Y(p0.mY - hp));
            cairo_line_to (mContext, r2p_X(p0.mX + wp), r2p_Y(p0.mY + hp));

            cairo_line_to (mContext, r2p_X(p1.mX + wp), r2p_Y(p1.mY + hp));
            cairo_line_to (mContext, r2p_X(p1.mX - wp), r2p_Y(p1.mY + hp));
            cairo_line_to (mContext, r2p_X(p1.mX - wp), r2p_Y(p1.mY - hp));

            cairo_line_to (mContext, r2p_X(p0.mX - wp), r2p_Y(p0.mY - hp));
            cairo_line_to (mContext, r2p_X(p0.mX + wp), r2p_Y(p0.mY - hp));
        }

        else if(p0.mX >= p1.mX && p0.mY >= p1.mY){
            cairo_move_to (mContext, r2p_X(p0.mX + wp), r2p_Y(p0.mY + hp));
            cairo_line_to (mContext, r2p_X(p0.mX - wp), r2p_Y(p0.mY + hp));

            cairo_line_to (mContext, r2p_X(p1.mX - wp), r2p_Y(p1.mY + hp));
            cairo_line_to (mContext, r2p_X(p1.mX - wp), r2p_Y(p1.mY - hp));
            cairo_line_to (mContext, r2p_X(p1.mX + wp), r2p_Y(p1.mY - hp));

            cairo_line_to (mContext, r2p_X(p0.mX + wp), r2p_Y(p0.mY - hp));
            cairo_line_to (mContext, r2p_X(p0.mX + wp), r2p_Y(p0.mY + hp));
        }

        else{
            err_printf("ERROR(CairoGerberViewer::drawDraw): p0/p1 relation not handled");
            return;
        }

        cairo_fill(mContext);
    }

    else{
        err_printf("ERROR(CairoGerberViewer::drawDraw): The aperture is not compatible");
    }

}
