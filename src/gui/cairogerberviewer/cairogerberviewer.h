#ifndef CAIROGERBERVIEWER_H
#define CAIROGERBERVIEWER_H


#include "igerberview.h"

#include <cairo/cairo.h>


class CairoGerberViewer: public IGerberView
{
    public:
        CairoGerberViewer();
        virtual ~CairoGerberViewer();

        virtual void drawAll(const GerberHandler &inGerber);
        virtual void drawLayer(GerberLayer *inLayer);


        void drawFlash(GraphicObjectFlash *inFlash);
        void drawAperturePrimitive(IAperturePrimitive *inPrim);
        void drawDraw(GraphicObjectDraw *inDraw);
        void drawArc(GraphicObjectArc *inArc);
        void drawRegion(GraphicObjectRegion *inRegion);


        virtual uint32_t getWidth() const;
        virtual uint32_t getHeight() const;


        cairo_surface_t *getSurface() {return mSurface;}


        void deinitCairo();

        /// resize the surface
        void initCairo(uint32_t inW, uint32_t inH);


    private:
        void setLevelPolarity(GraphicState::eLevelPolarity inPol);

        void drawAperturePrimitive_Circle(APrimCircle *inCircle);
        void drawAperturePrimitive_VectorLine(APrimVectorLine *inLine);
        void drawAperturePrimitive_CenterLine(APrimCenterLine *inLine);
        void drawAperturePrimitive_Outline(APrimOutline *inOutline);
        void drawAperturePrimitive_Polygon(APrimPolygon *inPoly);
        void drawAperturePrimitive_Moire(APrimMoire *inMoire);
        void drawAperturePrimitive_Thermal(APrimThermal *inThermal);





    private:
        cairo_t *mContext;
        cairo_surface_t *mSurface;


        IGerberView::ColorList mColorList;
};

#endif // CAIROGERBERVIEWER_H
