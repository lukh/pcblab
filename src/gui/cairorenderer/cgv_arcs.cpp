#include "cairogerberrenderer.h"

#include <math.h>


void CairoGerberRenderer::drawArc(GraphicObjectArc *inArc)
{
    if(mContext == NULL){ return; }

    if(inArc == NULL){ return; }

    if(!inArc->isValid()){
        err_printf("ERROR(CairoGerberViewer::drawArc): The arc is invalid");
        return;
    }



    Aperture *ap = inArc->getAperture();
    if(! ap->isValid() || ap->getPrimitives().size() < 1){
        err_printf("ERROR(CairoGerberViewer::drawArc): The aperture is invalid");
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
    }

    else{
        err_printf("ERROR(CairoGerberViewer::drawArc): The aperture must be a circle");
        return;
    }




    // --- draw ---

    double cx, cy, radius, angle1, angle2;

    cx = inArc->getCenter().mX;
    cy = inArc->getCenter().mY;

    //already check if the arc is valid
    radius = sqrt( pow(inArc->getStartPoint().mX - cx, 2) + pow(inArc->getStartPoint().mY - cy, 2) );

    angle1 = std::atan2(inArc->getStartPoint().mY - cy, inArc->getStartPoint().mX - cx);
    angle2 = std::atan2(inArc->getEndPoint().mY - cy, inArc->getEndPoint().mX - cx);

    switch(inArc->getInterpolationMode()){
        case GraphicState::eInterpolCWCircular:
            cairo_arc(mContext, cx, cy, radius, angle1, angle2);
            break;
        case GraphicState::eInterpolCCWCircular:
            cairo_arc_negative(mContext, cx, cy, radius, angle1, angle2);
            break;
        default:
            err_printf("ERROR(CairoGerberViewer::drawArc): The arc is invalid");
            break;
    }



    cairo_stroke(mContext);
}
