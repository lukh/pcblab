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


    vector<IAperturePrimitive *> primitives = ap->getPrimitives();
    for (vector<IAperturePrimitive *>::iterator it = primitives.begin(); it != primitives.end(); ++it){
        drawAperturePrimitive(*it);
    }
}

void CairoGerberViewer::drawAperturePrimitive(IAperturePrimitive *inPrim)
{
    if(inPrim == NULL){
        return;
    }

    //exposure
    //inPrim->getExposure();

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
