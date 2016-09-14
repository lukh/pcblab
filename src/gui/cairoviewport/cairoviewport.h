#ifndef CAIROVIEWPORT_H
#define CAIROVIEWPORT_H

#include <stdint.h>

#include <cairo/cairo.h>

#include "pcblab/common.h"

#include "icairoviewer.h"

/// the viewport is dedicated to render a specified area of a cairo_surface_t to another
class CairoViewport: public ICairoViewer
{
    public:
        CairoViewport();
        CairoViewport(uint32_t inW, uint32_t inH, cairo_surface_t *inSource);
        virtual ~CairoViewport();


        /// init the source context
        void setSource(cairo_surface_t *inSourceSurface) { mSourceSurface = inSourceSurface; }
        void setRenderTransformation(const plRectangle &inArea);

        plPoint getPointInSourceCoordinates(plPoint inImgCoord) const;
        void getVectorInSourceCoordinates(double *inDx, double *inDy) const;

        const plRectangle &getRenderArea() const { return mRenderArea; }


        /// render the source ctx to the dest ctx
        void refresh();

   protected:
        void applyTransformation();


    private:
        plRectangle mRenderArea;

        cairo_surface_t *mSourceSurface;
};

#endif // CAIROVIEWPORT_H
