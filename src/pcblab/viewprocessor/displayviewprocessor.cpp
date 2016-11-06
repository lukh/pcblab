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
    plRectangle rect(p1, p2);
    mViewport.setRenderArea(rect);

    mOriginalAreaViewport = rect;
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

    // define zoom
    if(inZoomIn) { zoomFactor = 1.1; }
    else { zoomFactor = 1/1.1; }


    // define existing area
    plRectangle rendererRect = mViewport.getRenderArea();
    rendererRect = mGerberRenderer.getRectInSourceCoords(rendererRect);

    static bool lastZoomInViewport = true;
    bool zoomInViewport = (double(rendererRect.getW())/double(getOriginalArea().getW())) > 0.3;

    // zoom in viewport
    if(zoomInViewport){
        plRectangle viewportRect;
        // define existing area
        if(lastZoomInViewport){
            viewportRect = mViewport.getRenderArea();
        }
        else{
            viewportRect = mGerberRenderer.getRectInImgCoords(mGerberRenderer.getRenderArea());
        }

        plPoint mousePos = mViewport.getPointInSourceCoords(plPoint(inPoint.mX, inPoint.mY));
        viewportRect = calculateZoom(zoomFactor, mousePos, viewportRect);

        mGerberRenderer.setRenderArea(getOriginalArea());
        mViewport.setRenderArea(viewportRect);


        if(!lastZoomInViewport){
            mGerberRenderer.drawAll(mPcb.getGerber());
        }
    }

    // zoom in gerber view
    else{
        // reset the gerberrenderer
        mViewport.setRenderArea(mOriginalAreaViewport);

        plPoint mousePos = mGerberRenderer.getPointInSourceCoords(mViewport.getPointInSourceCoords(plPoint(inPoint.mX, inPoint.mY)));
        rendererRect = calculateZoom(zoomFactor, mousePos, rendererRect);

        mGerberRenderer.setRenderArea(rendererRect);

        mGerberRenderer.drawAll(mPcb.getGerber());
    }

    mViewport.refresh();

    mCairoWidget->showImage(mViewport.getSurface());

    lastZoomInViewport = zoomInViewport;
}

void DisplayViewProcessor::move(double inDx, double inDy)
{
    //convert dx, dy into real coordinates
    mViewport.getVectorInSourceCoords(&inDx, &inDy);

    plRectangle rect = mViewport.getRenderArea();

    plRectangle n_rect = calculateMove(inDx, inDy, rect);

    mViewport.setRenderArea(n_rect);


    mGerberRenderer.drawAll(mPcb.getGerber());
    mViewport.refresh();

    mCairoWidget->showImage(mViewport.getSurface());
}


plPoint DisplayViewProcessor::convertCoordsFromImageToReal(plPoint inImgCoords)
{
    plPoint viewport_p = mViewport.getPointInSourceCoords(inImgCoords);

    return mGerberRenderer.getPointInSourceCoords(viewport_p);
}
