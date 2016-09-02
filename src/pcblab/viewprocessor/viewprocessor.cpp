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



