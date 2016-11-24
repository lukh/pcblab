#include "displayviewprocessor.h"

DisplayViewProcessor::DisplayViewProcessor(PcbLab &inPcb, ICairoWidget *inCairoWidget):
    IViewProcessor(inPcb), mCairoWidget(inCairoWidget)
{

}

void DisplayViewProcessor::init(uint32_t inWidth, uint32_t inHeight)
{
    // /////////////////////////////
    // init the viewer
    // /////////////////////////////
    setup(inWidth, inHeight);
}




void DisplayViewProcessor::refresh()
{
    mViewer.clean();
    mGerberRenderer.drawAll(mPcb.getGerber());
    mExcellonRenderer.draw(mPcb.getExcellon());
    mComponentRenderer.draw(mPcb.getComponents());

    mCairoWidget->showImage(mViewer.getSurface());
}



void DisplayViewProcessor::zoom(bool inZoomIn, plPoint inPoint)
{
    double zoomFactor;

    // define zoom
    if(inZoomIn) { zoomFactor = 1.2; }
    else { zoomFactor = 1/1.2; }

    plRectangle rendererRect = mViewer.getRenderArea();

    plPoint mousePos = mViewer.getPointInSourceCoords(inPoint);
    rendererRect = calculateZoom(zoomFactor, mousePos, rendererRect);

    mViewer.setRenderArea(rendererRect);

    mViewer.clean();
    mGerberRenderer.drawAll(mPcb.getGerber());
    mExcellonRenderer.draw(mPcb.getExcellon());
    mComponentRenderer.draw(mPcb.getComponents());

    mCairoWidget->showImage(mViewer.getSurface());
}

void DisplayViewProcessor::move(double inDx, double inDy)
{
    //convert dx, dy into real coordinates
    mViewer.getVectorInSourceCoords(&inDx, &inDy);

    plRectangle rect = mViewer.getRenderArea();

    plRectangle n_rect = calculateMove(inDx, inDy, rect);

    mViewer.setRenderArea(n_rect);

    mViewer.clean();
    mGerberRenderer.drawAll(mPcb.getGerber());
    mExcellonRenderer.draw(mPcb.getExcellon());
    mComponentRenderer.draw(mPcb.getComponents());

    mCairoWidget->showImage(mViewer.getSurface());
}


plPoint DisplayViewProcessor::convertCoordsFromImageToReal(plPoint inImgCoords)
{
    return mViewer.getPointInSourceCoords(inImgCoords);
}
