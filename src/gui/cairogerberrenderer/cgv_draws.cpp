#include "cairogerberrenderer.h"

#include <math.h>



void CairoGerberRenderer::drawDraw(GraphicObjectDraw *inDraw)
{
    plPoint p0 = inDraw->getStartPoint();
    plPoint p1 = inDraw->getEndPoint();

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
        cairo_set_line_width (mContext, width);

        // draw the line
        cairo_move_to (mContext, p0.mX, p0.mY);
        cairo_line_to (mContext, p1.mX, p1.mY);
        cairo_stroke (mContext);
    }

    else if(at.getName() == "R" && ip->getType() == IAperturePrimitive::eCenterLine){
        double wp = static_cast<APrimCenterLine *>(ip)->getWidth();
        double hp = static_cast<APrimCenterLine *>(ip)->getHeight();


        //draw shape
        if(p0.mX < p1.mX && p0.mY < p1.mY){
            cairo_move_to (mContext, p0.mX - wp, p0.mY - hp);
            cairo_line_to(mContext, p0.mX + wp, p0.mY - hp);

            cairo_line_to(mContext, p1.mX + wp, p1.mY - hp);
            cairo_line_to(mContext, p1.mX + wp, p1.mY + hp);
            cairo_line_to(mContext, p1.mX - wp, p1.mY + hp);

            cairo_line_to(mContext, p0.mX - wp, p0.mY + hp);
            cairo_line_to(mContext, p0.mX - wp, p0.mY - hp);
        }
        else if(p0.mX < p1.mX && p0.mY >= p1.mY){
            cairo_move_to (mContext, p0.mX - wp, p0.mY + hp);
            cairo_line_to (mContext, p0.mX - wp, p0.mY - hp);

            cairo_line_to (mContext, p1.mX - wp, p1.mY - hp);
            cairo_line_to (mContext, p1.mX + wp, p1.mY - hp);
            cairo_line_to (mContext, p1.mX + wp, p1.mY + hp);

            cairo_line_to (mContext, p0.mX + wp, p0.mY + hp);
            cairo_line_to (mContext, p0.mX - wp, p0.mY + hp);
        }

        else if(p0.mX >= p1.mX && p0.mY < p1.mY){
            cairo_move_to (mContext, p0.mX + wp, p0.mY - hp);
            cairo_line_to (mContext, p0.mX + wp, p0.mY + hp);

            cairo_line_to (mContext, p1.mX + wp, p1.mY + hp);
            cairo_line_to (mContext, p1.mX - wp, p1.mY + hp);
            cairo_line_to (mContext, p1.mX - wp, p1.mY - hp);

            cairo_line_to (mContext, p0.mX - wp, p0.mY - hp);
            cairo_line_to (mContext, p0.mX + wp, p0.mY - hp);
        }

        else if(p0.mX >= p1.mX && p0.mY >= p1.mY){
            cairo_move_to (mContext, p0.mX + wp, p0.mY + hp);
            cairo_line_to (mContext, p0.mX - wp, p0.mY + hp);

            cairo_line_to (mContext, p1.mX - wp, p1.mY + hp);
            cairo_line_to (mContext, p1.mX - wp, p1.mY - hp);
            cairo_line_to (mContext, p1.mX + wp, p1.mY - hp);

            cairo_line_to (mContext, p0.mX + wp, p0.mY - hp);
            cairo_line_to (mContext, p0.mX + wp, p0.mY + hp);
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
