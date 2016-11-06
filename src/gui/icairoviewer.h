#ifndef ICAIROVIEWER_H
#define ICAIROVIEWER_H

#include <stdint.h>

#include <cairo/cairo.h>

#include "pcblab/common.h"

/// Handles the low level of a cairo setup (surface and context)
/// and the lows level configuration
class ICairoViewer{
    public:
        ICairoViewer(): mContext(NULL), mSurface(NULL) {}
        virtual ~ICairoViewer() { deinitCairo(); }

        // -----------------------------------------
        // init, and surfaces access
        // -----------------------------------------


        uint32_t getWidth() const;

        uint32_t getHeight() const;

        /// returns the status of the renderer
        bool isViewerReady() const { return mContext != NULL && mSurface != NULL; }


        cairo_surface_t *getSurface() const { return mSurface; }


        void deinitCairo();

        /// resize the surface
        void initCairo(uint32_t inW, uint32_t inH);


        // -----------------------------------------
        // dedicated to rendering transformations
        // -----------------------------------------

        plPoint getPointInSourceCoords(plPoint inImgCoord) const;
        void getVectorInSourceCoords(double *inDx, double *inDy) const;
        plRectangle getRectInSourceCoords(plRectangle inImgRect) const;

        plPoint getPointInImgCoords(plPoint inSourceCoord) const;
        void getVectorInImgCoords(double *inDx, double *inDy) const;
        plRectangle getRectInImgCoords(plRectangle inSourceRect) const;



        /// zoom to fit the area given in the real world coordinates into the surface
        void setRenderArea(const plRectangle &inRealWorldArea) { mRenderArea = inRealWorldArea; }

        /// zoom to fit the area given in the real world coordinates into the surface
        void setRenderArea(plPoint p1, plPoint p2);

        /// returns the (real world) view area
        const plRectangle &getRenderArea() const { return mRenderArea; }


    protected:
        cairo_t *mContext;
        cairo_surface_t *mSurface;

        ///defines the area rendered on the surface
        plRectangle mRenderArea;



    private:
        virtual void applyRenderTransformation() = 0;

};


#endif // ICAIROVIEWER_H
