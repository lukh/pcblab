#include "icairorenderer.h"

void ICairoRenderer::setViewer(ICairoViewer &mViewer)
{
    mContext = mViewer.getContext();
}

