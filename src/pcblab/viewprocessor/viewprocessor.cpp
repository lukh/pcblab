#include <cairo/cairo.h>

#include "viewprocessor.h"



void CairoToMat(cairo_surface_t *surface,cv::Mat &MC3)
{
    cv::Mat MC4 = cv::Mat(cairo_image_surface_get_height(surface),cairo_image_surface_get_width(surface),CV_8UC4,cairo_image_surface_get_data(surface),cairo_image_surface_get_stride(surface));

    vector<cv::Mat> Imgs1;
    vector<cv::Mat> Imgs2;
    cv::split(MC4,Imgs1);
    cv::split(MC3,Imgs2);
    for(int i=0;i<3;i++)
    {
    Imgs2[i]=Imgs1[i];
    }
    cv::merge(Imgs2,MC3);
}





ViewProcessor::ViewProcessor(PcbLab &inPcb, IOpenCVViewer *inCVViewer, CairoWidget *inCairoWidget, CairoGerberViewer *inGerberView):
   mPcb(inPcb), mCVViewer(inCVViewer), mCairoWidget(inCairoWidget), mGerberViewer(inGerberView)
{
    mGerberViewer->initCairo(800, 500);
}

void ViewProcessor::update()
{
    mGerberViewer->drawAll(mPcb.getGerber());

    cairo_surface_t *surface = mGerberViewer->getSurface();

    cv::Mat m(mGerberViewer->getWidth(), mGerberViewer->getHeight(), CV_8UC3);
    CairoToMat(surface, m);
    mCVViewer->showImage(m);

    mCairoWidget->showImage(surface);
}



void ViewProcessor::updateZoom(bool inZoomIn, plPoint inPoint)
{
    double zoomFactor;

    if(inZoomIn) { zoomFactor = 1.1; }
    else { zoomFactor = 1/1.1; }


    plRectangle viewRect = mGerberViewer->getRealWorldViewArea();
    plPoint mousePos = mGerberViewer->getPointInRealWorldCoordinates(inPoint);
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

    /*
        First, find differences of size between zoomed rect and original rect
        Here, 1 / zoomFactor is used, because computations are made relative to the
        original view area, not the final rect):
    */


    double dX = viewRect.getW() * (1 - 1 / zoomFactor);
    double dY = viewRect.getH() * (1 - 1 / zoomFactor);

    /*
        Second, find d_* using the position of the mouse.
        pX = position of the mouse along X axis, relative to viewRect (percentage)
        pY = position of the mouse along Y axis, relative to viewRect (percentage)
        The value of d_right and d_down is not computed because is not directly needed
        in the final result.
    */
    double pX = (mousePos.mX - viewRect.getBottomLeft().mX) / viewRect.getW();
    double pY = (mousePos.mY - viewRect.getBottomLeft().mY) / viewRect.getH();

    double d_left = pX * dX;
    double d_up = pY * dY;

    /*
        Third and last, compute the output rect
    */
    plPoint p1 = viewRect.getBottomLeft(), p2=viewRect.getTopRight();

    //dX - d_left =  d_right
    //dY - d_up = d_down
    p1.mX += d_left;
    p1.mY += d_up;
    p2.mX -= dX - d_left;
    p2.mY -= dY - d_up;

    // That's it!


    mGerberViewer->setRenderTransformation(p1, p2);

    update();
}

void ViewProcessor::move(double inDx, double inDy)
{
    //convert dx, dy into real coordinates
    mGerberViewer->getVectorInRealWorldCoordinates(&inDx, &inDy);

    plRectangle rect = mGerberViewer->getRealWorldViewArea();

    plRectangle n_rect(rect.getX1() - inDx, rect.getY1() + inDy, rect.getX2() - inDx, rect.getY2() + inDy);

    mGerberViewer->setRenderTransformation(n_rect);

    update();
}



