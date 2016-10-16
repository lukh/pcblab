#ifndef ICAIROWIDGET_H
#define ICAIROWIDGET_H

#include <cairo/cairo.h>
#include "pcblab/common.h"

class ICairoWidget{
    public:
        ICairoWidget(){}

        virtual void showImage(cairo_surface_t *inSurface) = 0;
        virtual plPoint getCoordWidget2Img(plPoint inPoint) = 0;
};

#endif // ICAIROWIDGET_H
