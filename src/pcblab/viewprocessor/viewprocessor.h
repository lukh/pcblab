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




    private:
        PcbLab &mPcb;

        IOpenCVViewer *mCVViewer;
        CairoWidget *mCairoWidget;

        CairoGerberViewer *mGerberViewer;
};

#endif // VIEWPROCESSOR_H
