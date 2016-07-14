#ifndef VIEWPROCESSOR_H
#define VIEWPROCESSOR_H


#include "cairogerberviewer.h"
#include "iopencvviewer.h"

#include "gerberlayer.h"


class ViewProcessor
{
    public:
        ViewProcessor(IOpenCVViewer *inCVViewer, CairoGerberViewer *inGerberView);


        void update(const GerberLayer &inGerberLayer);

        void recalculateSize();

    private:
        IOpenCVViewer *mCVViewer;
        CairoGerberViewer *mGerberViewer;
};

#endif // VIEWPROCESSOR_H
