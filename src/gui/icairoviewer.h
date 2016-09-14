#ifndef ICAIROVIEWER_H
#define ICAIROVIEWER_H

#include <stdint.h>

#include <cairo/cairo.h>

class ICairoViewer{
    public:
        ICairoViewer(): mContext(NULL), mSurface(NULL) {}
        virtual ~ICairoViewer() { deinitCairo(); }

        uint32_t getWidth() const;

        uint32_t getHeight() const;

        /// returns the status of the renderer
        bool isViewerReady() const { return mContext != NULL && mSurface != NULL; }


        cairo_surface_t *getSurface() const { return mSurface; }


        void deinitCairo();

        /// resize the surface
        void initCairo(uint32_t inW, uint32_t inH);

    protected:
        cairo_t *mContext;
        cairo_surface_t *mSurface;

};


#endif // ICAIROVIEWER_H
