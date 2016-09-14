#ifndef DISPLAYVIEWPROCESSOR_H
#define DISPLAYVIEWPROCESSOR_H

# include <stdint.h>

#include "cairogerberrenderer.h"
#include "cairowidget.h"
#include "cairoviewport/cairoviewport.h"

#include "pcblab/pcblab.h"


class DisplayViewProcessor
{
    public:
        DisplayViewProcessor(PcbLab &inPcb, CairoWidget *inCairoWidget);


        /// init graphics,(cairo), rendering area, etc
        void init(uint32_t inWidth, uint32_t inHeight);


        /// repaint all the surface
        void refresh();

        ///update the view
        void update();


        /// related to zoom
        void updateZoom(bool inZoomIn, plPoint inPoint);

        /// move regarding the delta, in the surface coordinate
        void move(double inDx, double inDy);


    private:
        PcbLab &mPcb;

        CairoGerberRenderer mGerberRenderer;
        CairoViewport mViewport;

        CairoWidget *mCairoWidget; //should be interfaced
};

#endif // DISPLAYVIEWPROCESSOR_H
