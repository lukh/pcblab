#ifndef ICAIRORENDERER_H
#define ICAIRORENDERER_H

#include <cairo/cairo.h>

#include "pcblab/common.h"
#include "gui/icairoviewer.h"

class ICairoRenderer
{
    public:
        ICairoRenderer(): mContext(NULL), mFontOpts(NULL), mScaledFont(NULL) {}
        virtual ~ICairoRenderer() {}

        void setViewer(ICairoViewer &mViewer);

    protected:
        void initFont(double inFontSize = 15);
        void deinitFont();

    protected:
        cairo_t *mContext;

        cairo_font_options_t *mFontOpts;
        cairo_scaled_font_t *mScaledFont;
};

#endif // ICAIRORENDERER_H
