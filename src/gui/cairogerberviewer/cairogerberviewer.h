#ifndef CAIROGERBERVIEWER_H
#define CAIROGERBERVIEWER_H


#include "igerberview.h"

#include <cairo/cairo.h>


class CairoGerberViewer: public IGerberView
{
    public:
        CairoGerberViewer();
        virtual ~CairoGerberViewer();

        virtual void drawAll(/*TODO*/);
        virtual void drawLayer(GerberLayer &inLayer);


        void drawDraw(GraphicObjectDraw *inDraw);
        void drawArc(GraphicObjectArc *inArc);


        virtual uint32_t getWidth();
        virtual uint32_t getHeight();


        cairo_surface_t *getSurface() {return mSurface;}


        void deinitCairo();

        /// resize the surface
        void initCairo(uint32_t inW, uint32_t inH);


    private:
        void setLevelPolarity(GraphicState::eLevelPolarity inPol);
        void setDarkColor(uint8_t inR, uint8_t inG, uint8_t inB) { mColor = Color(inR,inG,inB); }

    private:
        cairo_t *mContext;
        cairo_surface_t *mSurface;


        // used for dark
        Color mColor;
};

#endif // CAIROGERBERVIEWER_H
