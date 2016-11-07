#ifndef DISPLAYVIEWPROCESSOR_H
#define DISPLAYVIEWPROCESSOR_H

# include <stdint.h>

#include "iviewprocessor.h"

#include "icairowidget.h"
#include "cairoviewport/cairoviewport.h"



/// The display view processor handles all the processing to display a PcbLab project, in the same manner as a CAD software.
/// It takes the PcbLab instance and the cairo widget for the rendering.
class DisplayViewProcessor: public IViewProcessor
{
    public:
        DisplayViewProcessor(PcbLab &inPcb, ICairoWidget *inCairoWidget);
        virtual ~DisplayViewProcessor() {}


        /// init graphics,(cairo), rendering area, etc
        void init(uint32_t inWidth, uint32_t inHeight);


        //<<< --- Interface of IViewProcessor
        /// update the view, change the transformations
        virtual void refresh();

        /// related to zoom
        virtual void zoom(bool inZoomIn, plPoint inPoint);

        /// move regarding the delta, in the surface coordinate
        virtual void move(double inDx, double inDy);


        /// convert coords from image (pixels) to reality (mm/in)
        virtual plPoint convertCoordsFromImageToReal(plPoint inImgCoords);
        // --- >>>


    private:
        ICairoWidget *mCairoWidget;
};

#endif // DISPLAYVIEWPROCESSOR_H
