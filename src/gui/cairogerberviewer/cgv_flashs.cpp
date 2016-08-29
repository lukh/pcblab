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
    if(ap->isValid()){
        err_printf("ERROR(CairoGerberViewer::drawFlash): The flash aperture is invalid");
        return;
    }

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

}

void CairoGerberViewer::drawAperturePrimitive_VectorLine(APrimVectorLine *inLine)
{

}

void CairoGerberViewer::drawAperturePrimitive_CenterLine(APrimCenterLine *inLine)
{

}

void CairoGerberViewer::drawAperturePrimitive_Outline(APrimOutline *inOutline)
{

}

void CairoGerberViewer::drawAperturePrimitive_Polygon(APrimPolygon *inPoly)
{

}

void CairoGerberViewer::drawAperturePrimitive_Moire(APrimMoire *inMoire)
{

}

void CairoGerberViewer::drawAperturePrimitive_Thermal(APrimThermal *inThermal)
{

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
