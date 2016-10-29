#ifndef DISPLAYVIEWPROCESSOR_H
#define DISPLAYVIEWPROCESSOR_H

# include <stdint.h>

#include "cairogerberrenderer.h"
#include "icairowidget.h"
#include "cairoviewport/cairoviewport.h"

#include "pcblab/pcblab.h"


/// The display view processor handles all the processing to display a PcbLab project, in the same manner as a CAD software.
/// It takes the PcbLab instance and the cairo widget for the rendering.
class DisplayViewProcessor
{
    public:
        DisplayViewProcessor(PcbLab &inPcb, ICairoWidget *inCairoWidget);


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



        /// convert coords from image (pixels) to reality (mm/in)
        plPoint convertCoordsFromImageToReal(plPoint inImgCoords);

    private:
        PcbLab &mPcb;

        CairoGerberRenderer mGerberRenderer;
        CairoViewport mViewport;

        ICairoWidget *mCairoWidget;
};

#endif // DISPLAYVIEWPROCESSOR_H
