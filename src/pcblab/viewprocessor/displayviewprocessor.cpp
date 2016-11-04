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

    // /////////////////////////////
    // init the viewport
    // /////////////////////////////
    mViewport.initCairo(inWidth, inHeight);
    mViewport.setSource(mGerberRenderer.getSurface());

    plPoint p1(0,0), p2(mGerberRenderer.getWidth(), mGerberRenderer.getHeight());
    mViewport.setRenderArea(plRectangle(p1, p2));
}




void DisplayViewProcessor::refresh()
{
    mGerberRenderer.drawAll(mPcb.getGerber());

    mViewport.refresh();

    mCairoWidget->showImage(mViewport.getSurface());
}



void DisplayViewProcessor::zoom(bool inZoomIn, plPoint inPoint)
{
   double zoomFactor;

   if(inZoomIn) { zoomFactor = 1.1; }
   else { zoomFactor = 1/1.1; }

   plRectangle viewRect = mViewport.getRenderArea();
   plPoint mousePos = mViewport.getPointInSourceCoords(plPoint(inPoint.mX, inPoint.mY));


   viewRect = calculateZoom(zoomFactor, mousePos, viewRect);

   mViewport.setRenderArea(viewRect);

   refresh();
}

void DisplayViewProcessor::move(double inDx, double inDy)
{
    //convert dx, dy into real coordinates
    mViewport.getVectorInSourceCoords(&inDx, &inDy);

    plRectangle rect = mViewport.getRenderArea();

    plRectangle n_rect = calculateMove(inDx, inDy, rect);

    mViewport.setRenderArea(n_rect);

    refresh();
}


plPoint DisplayViewProcessor::convertCoordsFromImageToReal(plPoint inImgCoords)
{
    plPoint viewport_p = mViewport.getPointInSourceCoords(inImgCoords);

    return mGerberRenderer.getPointInSourceCoords(viewport_p);
}
