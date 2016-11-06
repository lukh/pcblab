#include "iviewprocessor.h"

IViewProcessor::IViewProcessor(PcbLab &inPcb): mPcb(inPcb)
{

}


void IViewProcessor::updateLayerColor(string inIdentifier, Color inColor)
{
    mGerberRenderer.setColor(inIdentifier, inColor);
    refresh();
}

void IViewProcessor::updateLayerTransparency(string inIdentifier, uint8_t inTransp)
{
    mGerberRenderer.setAlphaChannel(inIdentifier, inTransp);
    refresh();
}

void IViewProcessor::setup(uint32_t inWidth, uint32_t inHeight)
{
    plRectangle r_real = mPcb.getGerber().getBoundingBox();
    //get some headroom..
    plRectangle r_real_hr(
        plPoint(r_real.getBottomLeft().mX - 10.0, r_real.getBottomLeft().mY - 10.0),
        plPoint(r_real.getTopRight().mX + 10.0, r_real.getTopRight().mY + 10.0)
    );

    mOriginalArea = r_real_hr;

    // /////////////////////////////
    // init the renderer
    // /////////////////////////////
    uint32_t rw = inWidth;
    uint32_t rh = inHeight;
    mGerberRenderer.initCairo(rw, rh);
    mGerberRenderer.setRenderArea(r_real_hr);
    //reset the graphics settings
    IGerberView::GraphicSettings::reset();
}

plRectangle IViewProcessor::calculateZoom(double inZoomFactor, plPoint inPoint, plRectangle inRect)
{
    double zoomFactor = inZoomFactor;
    plRectangle viewRect = inRect;
    plPoint mousePos = inPoint;

    //
    /*
     * from: http://stackoverflow.com/questions/13316481/zooming-into-a-window-based-on-the-mouse-position
        VARIABLES (all in space coordinates, not pixel coordinates):

          input:
            viewRect = rectangle of the viewed area
            zoomFactor = factor of zoom relative to viewRect, ex 1.1
            mousePos = position of the mouse

          output:
            zoomedRect = viexRect after zoom
    */

    /*
        A little schema:

          viewRect
        *-----------------------------------------------------------------------*
        |                       ^                                               |
        |                       | d_up                                          |
        |        zoomedRect     v                                               |
        |      *-----------------------------------------*                      |
        |d_left|                                         |       d_right        |
        |<---->|                mousePos                 |<-------------------->|
        |      |                    +                    |                      |
        |      |                                         |                      |
        |      |                                         |                      |
        |      *-----------------------------------------*                      |
        |                       ^                                               |
        |                       |                                               |
        |                       |                                               |
        |                       | d_down                                        |
        |                       |                                               |
        |                       v                                               |
        *-----------------------------------------------------------------------*

        dX = d_left + d_right
        dY = d_up + d_down
        The origin of rects is the upper left corner.
    */


    //    First, find differences of size between zoomed rect and original rect
    //    Here, 1 / zoomFactor is used, because computations are made relative to the
    //    original view area, not the final rect):



    double dX = viewRect.getW() * (1 - 1 / zoomFactor);
    double dY = viewRect.getH() * (1 - 1 / zoomFactor);


    //    Second, find d_* using the position of the mouse.
    //    pX = position of the mouse along X axis, relative to viewRect (percentage)
    //    pY = position of the mouse along Y axis, relative to viewRect (percentage)
    //    The value of d_right and d_down is not computed because is not directly needed
    //    in the final result.

    double pX = (mousePos.mX - viewRect.getBottomLeft().mX) / viewRect.getW();
    double pY = (mousePos.mY - viewRect.getBottomLeft().mY) / viewRect.getH();


    double d_left = pX * dX;
    double d_up = pY * dY;

    //  Third and last, compute the output rect
    plPoint p1 = viewRect.getBottomLeft(), p2=viewRect.getTopRight();

    double d_right = dX - d_left;
    double d_down = dY - d_up;

    p1.mX += d_left;
    p1.mY += d_up;

    p2.mX -= d_right;
    p2.mY -= d_down;

    // That's it!


    viewRect = plRectangle(p1, p2);

    return viewRect;
}

plRectangle IViewProcessor::calculateMove(double inDx, double inDy, plRectangle inRect)
{
    plRectangle rect = inRect;

    return plRectangle(rect.getX1() - inDx, rect.getY1() - inDy, rect.getX2() - inDx, rect.getY2() - inDy);
}
