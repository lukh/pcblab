#include "displayviewprocessor.h"

DisplayViewProcessor::DisplayViewProcessor(PcbLab &inPcb, ICairoWidget *inCairoWidget):
    IViewProcessor(inPcb), mCairoWidget(inCairoWidget)
{

}

void DisplayViewProcessor::init(uint32_t inWidth, uint32_t inHeight)
{
    // /////////////////////////////
    // init the renderer
    // /////////////////////////////
    setup(inWidth, inHeight);
}




void DisplayViewProcessor::refresh()
{
    mGerberRenderer.drawAll(mPcb.getGerber());
    mCairoWidget->showImage(mGerberRenderer.getSurface());
}



void DisplayViewProcessor::zoom(bool inZoomIn, plPoint inPoint)
{
    double zoomFactor;

    // define zoom
    if(inZoomIn) { zoomFactor = 1.1; }
    else { zoomFactor = 1/1.1; }

    plRectangle rendererRect = mGerberRenderer.getRenderArea();

    plPoint mousePos = mGerberRenderer.getPointInSourceCoords(inPoint);
    rendererRect = calculateZoom(zoomFactor, mousePos, rendererRect);

    mGerberRenderer.setRenderArea(rendererRect);

    mGerberRenderer.drawAll(mPcb.getGerber());


    mCairoWidget->showImage(mGerberRenderer.getSurface());
}

void DisplayViewProcessor::move(double inDx, double inDy)
{
    //convert dx, dy into real coordinates
    mGerberRenderer.getVectorInSourceCoords(&inDx, &inDy);

    plRectangle rect = mGerberRenderer.getRenderArea();

    plRectangle n_rect = calculateMove(inDx, inDy, rect);

    mGerberRenderer.setRenderArea(n_rect);


    mGerberRenderer.drawAll(mPcb.getGerber());

    mCairoWidget->showImage(mGerberRenderer.getSurface());
}


plPoint DisplayViewProcessor::convertCoordsFromImageToReal(plPoint inImgCoords)
{
    return mGerberRenderer.getPointInSourceCoords(inImgCoords);
}
