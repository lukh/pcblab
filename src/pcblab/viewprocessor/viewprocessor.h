#ifndef VIEWPROCESSOR_H
#define VIEWPROCESSOR_H


#include "cairogerberviewer.h"
#include "iopencvviewer.h"

#include "pcblab/pcblab.h"


class ViewProcessor
{
    public:
        ViewProcessor(PcbLab &inPcb, IOpenCVViewer *inCVViewer, CairoGerberViewer *inGerberView);


        void update();

        // temp method since the arch is not fully finished
        void update(GerberLayer &inGerberLayer);

        void recalculateSize();

    private:
        PcbLab &mPcb;
        IOpenCVViewer *mCVViewer;
        CairoGerberViewer *mGerberViewer;
};

#endif // VIEWPROCESSOR_H
