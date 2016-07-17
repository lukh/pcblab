#include "cairogerberviewer.h"

#include <math.h>

CairoGerberViewer::CairoGerberViewer(): IGerberView()
{
    mContext = NULL;
    mSurface = NULL;

    mColor = Color(255,0,0);
}

CairoGerberViewer::~CairoGerberViewer(){

}

void CairoGerberViewer::drawAll()
{

}

void CairoGerberViewer::drawLayer(GerberLayer &inLayer)
{
    vector<GerberLayer::GerberLevel *>levels = inLayer.getLevels();


    for (vector<GerberLayer::GerberLevel *>::iterator it_lvl = levels.begin(); it_lvl != levels.end(); ++it_lvl){
        GerberLayer::GerberLevel *level = *it_lvl;

        setLevelPolarity(level->getPolarity());

        vector<IGraphicObject *> gos = level->getObjects();
        for(vector<IGraphicObject *>::iterator it_go = gos.begin(); it_go != gos.end(); ++it_go){
            IGraphicObject *object = *it_go;

            switch(object->getType()){
                case IGraphicObject::eTypeLine:{
                    GraphicObjectDraw *draw = static_cast<GraphicObjectDraw *>(object);
                    drawDraw(draw);
                    break;}
                case IGraphicObject::eTypeArc:{
                    break;}
                case IGraphicObject::eTypeFlash:{
                    break;}
                case IGraphicObject::eTypeRegion:{
                    break;}
                default:
                    break;
            }
        }
    }

}

void CairoGerberViewer::drawDraw(GraphicObjectDraw *inDraw)
{
    Point p0 = inDraw->getStartPoint();
    Point p1 = inDraw->getEndPoint();



    if(mContext == NULL){
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

void CairoGerberViewer::drawArc(GraphicObjectArc *inArc)
{
}

uint32_t CairoGerberViewer::getWidth()
{
    if(mSurface == NULL){
        return 0;
    }

    return cairo_image_surface_get_width(mSurface);
}

uint32_t CairoGerberViewer::getHeight()
{
    if(mSurface == NULL){
        return 0;
    }

    return cairo_image_surface_get_height(mSurface);
}

void CairoGerberViewer::deinitCairo()
{
    if(mContext != NULL){
        cairo_destroy(mContext);
    }
    if(mSurface != NULL){
        cairo_surface_destroy(mSurface);
    }

    mContext = NULL;
    mSurface = NULL;
}

void CairoGerberViewer::initCairo(uint32_t inW, uint32_t inH)
{
    deinitCairo();

    if(mSurface == NULL){
        mSurface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, inW, inH);
    }

    if(mContext == NULL){
        mContext = cairo_create (mSurface);
    }
}

void CairoGerberViewer::setLevelPolarity(GraphicState::eLevelPolarity inPol)
{
    if(mContext == NULL){
        return;
    }

    if(inPol == GraphicState::ePolDark){
        cairo_set_source_rgb(mContext, (double)mColor.mR/255.0, (double)mColor.mG/255.0, (double)mColor.mB/255.0);
    }
    else{
        cairo_set_source_rgb(mContext, 0,0,0);
    }
}

