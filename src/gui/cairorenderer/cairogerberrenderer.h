#ifndef CAIROGERBERRENDERER_H
#define CAIROGERBERRENDERER_H


#include "igerberrenderer.h"
#include "icairoviewer.h"
#include "icairorenderer.h"

#include <cairo/cairo.h>


/// Dedicated to render a gerberhandler on a cairo surface.
class CairoGerberRenderer: public IGerberRenderer, public ICairoRenderer
{
    public:
        CairoGerberRenderer();
        virtual ~CairoGerberRenderer();


        //<<< --- Implementation of IGerberView
        virtual void drawAll(const GerberHandler &inGerber);
        virtual void drawLayer(string inIdentifier, const GerberLayer *inLayer);
        //--- >>>






    private:
        //<<< --- Implementation of IGerberView
        virtual void setLevelPolarity(GraphicState::eLevelPolarity inPol);

        virtual void drawDraw(GraphicObjectDraw *inDraw);
        virtual void drawArc(GraphicObjectArc *inArc);
        virtual void drawRegion(GraphicObjectRegion *inRegion);

        virtual void drawFlash(GraphicObjectFlash *inFlash);
        virtual void drawAperturePrimitive(IAperturePrimitive *inPrim);

        virtual void drawAperturePrimitive_Circle(APrimCircle *inCircle);
        virtual void drawAperturePrimitive_VectorLine(APrimVectorLine *inLine);
        virtual void drawAperturePrimitive_CenterLine(APrimCenterLine *inLine);
        virtual void drawAperturePrimitive_Outline(APrimOutline *inOutline);
        virtual void drawAperturePrimitive_Polygon(APrimPolygon *inPoly);
        virtual void drawAperturePrimitive_Moire(APrimMoire *inMoire);
        virtual void drawAperturePrimitive_Thermal(APrimThermal *inThermal);


        /// changes the exposure of the primitive
        virtual void setApertureExposure(IAperturePrimitive::eExposure inExposure);

        /// update the actual context with a rotation in degree for the primitive
        virtual void setApertureRotation(double inAngle);
        //--- >>>
};

#endif // CAIROGERBERVIEWER_H
