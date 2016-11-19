#ifndef ICAIRORENDERER_H
#define ICAIRORENDERER_H

#include <cairo/cairo.h>

#include "pcblab/common.h"
#include "gui/icairoviewer.h"

class ICairoRenderer
{
    public:
        ICairoRenderer(): mContext(NULL) {}
        virtual ~ICairoRenderer() {}

        void setViewer(ICairoViewer &mViewer);

    protected:
        cairo_t *mContext;
};

#endif // ICAIRORENDERER_H
