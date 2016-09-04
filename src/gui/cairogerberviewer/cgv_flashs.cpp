#include "cairogerberviewer.h"

#include <math.h>



void CairoGerberViewer::drawFlash(GraphicObjectFlash *inFlash)
{
    if(!isViewerReady()){ return; }

    if(inFlash == NULL){ return; }

    if(!inFlash->isValid()){
        err_printf("ERROR(CairoGerberViewer::drawFlash): The flash is invalid");
        return;
    }

    Aperture *ap = inFlash->getAperture();
    if(ap == NULL){
        err_printf("ERROR(CairoGerberViewer::drawFlash): The flash aperture is NULL");
        return;
    }
    if(! ap->isValid()){
        err_printf("ERROR(CairoGerberViewer::drawFlash): The flash aperture is invalid");
        return;
    }

    // save the context to draw all the prims from 0,0 point.
    cairo_save(mContext);

    // create a new group for the flash
    cairo_push_group(mContext);

    //move to the point 0,0 of the flash
    cairo_translate(mContext, inFlash->getPosition().mX, inFlash->getPosition().mY);

    vector<IAperturePrimitive *> primitives = ap->getPrimitives();
    for (vector<IAperturePrimitive *>::iterator it = primitives.begin(); it != primitives.end(); ++it){
        IAperturePrimitive *prim = *it;

        // save the context to draw all the prims from 0,0 point.
        cairo_save(mContext);

        //rotation
        setApertureRotation(prim->getRot());

        //exposure
        setApertureExposure(prim->getExposure());

        //draw
        drawAperturePrimitive(prim);

        cairo_restore(mContext);
    }

    // pop the group and paint
    cairo_pop_group_to_source(mContext);
    cairo_paint(mContext);

    cairo_restore(mContext);
}

void CairoGerberViewer::drawAperturePrimitive(IAperturePrimitive *inPrim)
{
    if(inPrim == NULL){
        return;
    }

    if(!inPrim->isValid()){ return; }

    switch(inPrim->getType()){
        case IAperturePrimitive::eCircle:
            drawAperturePrimitive_Circle(static_cast<APrimCircle*>(inPrim));
            break;
        case IAperturePrimitive::eVectorLine:
            drawAperturePrimitive_VectorLine(static_cast<APrimVectorLine*>(inPrim));
            break;
        case IAperturePrimitive::eCenterLine:
            drawAperturePrimitive_CenterLine(static_cast<APrimCenterLine*>(inPrim));
            break;
        case IAperturePrimitive::eOutLine:
            drawAperturePrimitive_Outline(static_cast<APrimOutline*>(inPrim));
            break;
        case IAperturePrimitive::ePolygon:
            drawAperturePrimitive_Polygon(static_cast<APrimPolygon*>(inPrim));
            break;
        case IAperturePrimitive::eMoire:
            drawAperturePrimitive_Moire(static_cast<APrimMoire*>(inPrim));
            break;
        case IAperturePrimitive::eThermal:
            drawAperturePrimitive_Thermal(static_cast<APrimThermal*>(inPrim));
            break;
        case IAperturePrimitive::eInvalid:
            err_printf("ERROR(CairoGerberViewer::drawAperturePrimitive): Primitive is invalid");
            break;
    }
}


void CairoGerberViewer::drawAperturePrimitive_Circle(APrimCircle *inCircle)
{
    //draw circle
    cairo_move_to(mContext, inCircle->getX() + inCircle->getDiameter()/2, inCircle->getY());
    cairo_arc (mContext, inCircle->getX(), inCircle->getY(), inCircle->getDiameter()/2, 0.0, 2 * M_PI);

    cairo_fill(mContext);
}

void CairoGerberViewer::drawAperturePrimitive_VectorLine(APrimVectorLine *inLine)
{
    cairo_set_line_cap(mContext, CAIRO_LINE_CAP_BUTT);

    //set the line
    /*double ux=inLine->getWidth(), uy=inLine->getWidth();
    cairo_device_to_user_distance (mContext, &ux, &uy);
    if (ux < uy) ux = uy;*/
    cairo_set_line_width (mContext, inLine->getWidth());

    // draw the line
    cairo_move_to (mContext, inLine->getStartX(), inLine->getStartY());
    cairo_line_to (mContext, inLine->getEndX(), inLine->getEndY());
    cairo_stroke (mContext);
}

void CairoGerberViewer::drawAperturePrimitive_CenterLine(APrimCenterLine *inLine)
{
    cairo_move_to(mContext, inLine->getX() - inLine->getWidth()/2.0, inLine->getY() - inLine->getHeight()/2.0);

    cairo_line_to(mContext, inLine->getX() + inLine->getWidth()/2.0, inLine->getY() - inLine->getHeight()/2.0);
    cairo_line_to(mContext, inLine->getX() + inLine->getWidth()/2.0, inLine->getY() + inLine->getHeight()/2.0);
    cairo_line_to(mContext, inLine->getX() - inLine->getWidth()/2.0, inLine->getY() + inLine->getHeight()/2.0);
    cairo_line_to(mContext, inLine->getX() - inLine->getWidth()/2.0, inLine->getY() - inLine->getHeight()/2.0);

    cairo_fill(mContext);
}

void CairoGerberViewer::drawAperturePrimitive_Outline(APrimOutline *inOutline)
{
    if(inOutline->getSubSequentPointsCount() == 0) { return; }

    for(uint16_t p_idx = 0; p_idx < inOutline->getSubSequentPointsCount(); p_idx ++){
        plPoint p = inOutline->getPoint(p_idx);

        if(p_idx == 0){ cairo_move_to(mContext, p.mX, p.mY); }

        else { cairo_line_to(mContext, p.mX, p.mY); }
    }

    cairo_fill(mContext);
}

void CairoGerberViewer::drawAperturePrimitive_Polygon(APrimPolygon *inPoly)
{
    double part_angle = 360.0 / inPoly->getVerticesCount();
    cairo_move_to(mContext, inPoly->getX(), inPoly->getY());

    cairo_save(mContext);
    for(uint16_t vert_idx = 0; vert_idx < inPoly->getVerticesCount(); vert_idx++){
        cairo_rotate(mContext, M_PI * part_angle / 180);
        cairo_line_to(mContext, inPoly->getDiameter()/2.0, 0.0);
    }
    cairo_restore(mContext);

    cairo_fill(mContext);
}

void CairoGerberViewer::drawAperturePrimitive_Moire(APrimMoire *inMoire)
{
    double dia = inMoire->getOuterDiaOfOutRing() - inMoire->getRingThickness();


    cairo_set_line_cap(mContext, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(mContext, CAIRO_LINE_JOIN_ROUND);
    /*double ux=inMoire->getRingThickness(), uy=inMoire->getRingThickness();
    cairo_device_to_user_distance (mContext, &ux, &uy);
    if (ux < uy){ ux = uy; }*/
    cairo_set_line_width (mContext, inMoire->getRingThickness());

    //draw the circles
    uint16_t num_rings=0;
    while (dia >= 0 && num_rings < inMoire->getMaxNumOfRings()){
        cairo_move_to(mContext, inMoire->getX()+dia/2.0, inMoire->getY());
        cairo_arc(mContext, inMoire->getX(), inMoire->getY(), dia/2.0, 0.0, 2*M_PI);
        cairo_stroke(mContext);

        dia -= 2.0*(inMoire->getGapBtwRings() + inMoire->getRingThickness());
        num_rings++;
    }

    //draw cross
    //set the line
    cairo_set_line_cap(mContext, CAIRO_LINE_CAP_BUTT);
    cairo_set_line_join(mContext, CAIRO_LINE_JOIN_BEVEL);
    /*ux=uy=inMoire->getCrossHaireThickness();
    cairo_device_to_user_distance (mContext, &ux, &uy);
    if (ux < uy){ ux = uy; }*/
    cairo_set_line_width (mContext, inMoire->getCrossHaireThickness());



    cairo_move_to(mContext, inMoire->getX(), inMoire->getY()-inMoire->getCrossHairLength()/2.0);
    cairo_line_to(mContext, inMoire->getX(), inMoire->getY()+inMoire->getCrossHairLength()/2.0);
    cairo_stroke(mContext);

    cairo_move_to(mContext, inMoire->getX()-inMoire->getCrossHairLength()/2.0, inMoire->getY());
    cairo_line_to(mContext, inMoire->getX()+inMoire->getCrossHairLength()/2.0, inMoire->getY());
    cairo_stroke(mContext);
}

void CairoGerberViewer::drawAperturePrimitive_Thermal(APrimThermal *inThermal)
{
    //draw outer circle
    cairo_set_operator(mContext, CAIRO_OPERATOR_OVER);
    cairo_move_to(mContext, inThermal->getX()+inThermal->getOuterDia()/2.0, inThermal->getY());
    cairo_arc(mContext, inThermal->getX(), inThermal->getY(), inThermal->getOuterDia()/2.0, 0.0, 2*M_PI);
    cairo_fill(mContext);



    cairo_set_operator(mContext, CAIRO_OPERATOR_CLEAR);

    // ----- remove inner circle ------
    cairo_move_to(mContext, inThermal->getX()+inThermal->getInnerDia()/2.0, inThermal->getY());
    cairo_arc(mContext, inThermal->getX(), inThermal->getY(), inThermal->getInnerDia()/2.0, 0.0, 2*M_PI);
    cairo_fill(mContext);

    //----- remove cross --------
    //set the line
    cairo_set_line_cap(mContext, CAIRO_LINE_CAP_BUTT);
    cairo_set_line_join(mContext, CAIRO_LINE_JOIN_BEVEL);
    /*double ux=inThermal->getGapThickness(), uy=inThermal->getGapThickness();
    cairo_device_to_user_distance (mContext, &ux, &uy);
    if (ux < uy){ ux = uy; }*/
    cairo_set_line_width (mContext, inThermal->getGapThickness());



    cairo_move_to(mContext, inThermal->getX(), inThermal->getY()-inThermal->getOuterDia()/2.0);
    cairo_line_to(mContext, inThermal->getX(), inThermal->getY()+inThermal->getOuterDia()/2.0);
    cairo_stroke(mContext);

    cairo_move_to(mContext, inThermal->getX()-inThermal->getOuterDia()/2.0, inThermal->getY());
    cairo_line_to(mContext, inThermal->getX()+inThermal->getOuterDia()/2.0, inThermal->getY());
    cairo_stroke(mContext);
}

void CairoGerberViewer::setApertureExposure(IAperturePrimitive::eExposure inExposure)
{
    if(!isViewerReady()){ return; }

    if(inExposure == IAperturePrimitive::eExposureOn){
        cairo_set_operator (mContext, CAIRO_OPERATOR_OVER);
    }
    else{
        cairo_set_operator (mContext, CAIRO_OPERATOR_CLEAR);
    }
}

void CairoGerberViewer::setApertureRotation(double inAngle)
{
    if(!isViewerReady()){ return; }

    cairo_rotate(mContext, inAngle * M_PI / 180.0);
}
