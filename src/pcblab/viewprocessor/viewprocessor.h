#ifndef VIEWPROCESSOR_H
#define VIEWPROCESSOR_H


#include "cairogerberviewer.h"
#include "iopencvviewer.h"
#include "cairowidget.h"

#include "pcblab/pcblab.h"


class ViewProcessor
{
    public:
        ViewProcessor(PcbLab &inPcb, IOpenCVViewer *inCVViewer, CairoWidget *inCairoWidget, CairoGerberViewer *inGerberView);


        void update();


        //related to zoom
        void updateZoom(bool inZoomIn, plPoint inPoint);

        /// move regarding the delta, in the surface coordinate
        void move(double inDx, double inDy);


    private:
        PcbLab &mPcb;

        IOpenCVViewer *mCVViewer;
        CairoWidget *mCairoWidget;

        CairoGerberViewer *mGerberViewer;
};

#endif // VIEWPROCESSOR_H
