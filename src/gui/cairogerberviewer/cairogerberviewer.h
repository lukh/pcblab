#ifndef CAIROGERBERVIEWER_H
#define CAIROGERBERVIEWER_H


#include "igerberview.h"

#include <cairo/cairo.h>


class CairoGerberViewer: public IGerberView
{
    public:
        CairoGerberViewer();
        virtual ~CairoGerberViewer();


        //<<< --- Implementation of IGerberView
        virtual void drawAll(const GerberHandler &inGerber);
        virtual void drawLayer(const GerberLayer *inLayer);

        virtual uint32_t getWidth() const;
        virtual uint32_t getHeight() const;

        /// returns the status of the renderer
        virtual bool isViewerReady() const { return mContext != NULL && mSurface != NULL; }


        virtual plPoint getPointInRealWorldCoordinates(plPoint inImgCoord) const;
        virtual void getVectorInRealWorldCoordinates(double *inDx, double *inDy) const;
        //--- >>>


        cairo_surface_t *getSurface() const { return mSurface; }


        void deinitCairo();

        /// resize the surface
        void initCairo(uint32_t inW, uint32_t inH);



    private:
        void applyRenderTransformation();

        void setLevelPolarity(GraphicState::eLevelPolarity inPol);

        void drawDraw(GraphicObjectDraw *inDraw);
        void drawArc(GraphicObjectArc *inArc);
        void drawRegion(GraphicObjectRegion *inRegion);

        void drawFlash(GraphicObjectFlash *inFlash);
        void drawAperturePrimitive(IAperturePrimitive *inPrim);

        void drawAperturePrimitive_Circle(APrimCircle *inCircle);
        void drawAperturePrimitive_VectorLine(APrimVectorLine *inLine);
        void drawAperturePrimitive_CenterLine(APrimCenterLine *inLine);
        void drawAperturePrimitive_Outline(APrimOutline *inOutline);
        void drawAperturePrimitive_Polygon(APrimPolygon *inPoly);
        void drawAperturePrimitive_Moire(APrimMoire *inMoire);
        void drawAperturePrimitive_Thermal(APrimThermal *inThermal);


        /// changes the exposure of the primitive
        void setApertureExposure(IAperturePrimitive::eExposure inExposure);

        /// update the actual context with a rotation in degree for the primitive
        void setApertureRotation(double inAngle);



    private:
        cairo_t *mContext;
        cairo_surface_t *mSurface;

};

#endif // CAIROGERBERVIEWER_H
