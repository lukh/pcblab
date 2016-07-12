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
        virtual void drawLayer(const GerberLayer &inLayer);

        virtual uint32_t getWidth();
        virtual uint32_t getHeight();


        cairo_surface_t *getSurface() {return mSurface;}


    private:
        cairo_t *mContext;
        cairo_surface_t *mSurface;
};

#endif // CAIROGERBERVIEWER_H
