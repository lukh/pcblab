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





ViewProcessor::ViewProcessor(PcbLab &inPcb, IOpenCVViewer *inCVViewer, CairoGerberViewer *inGerberView):
   mPcb(inPcb), mCVViewer(inCVViewer), mGerberViewer(inGerberView)
{

}

void ViewProcessor::update()
{
    recalculateSize();

    mGerberViewer->drawAll(mPcb.getGerber());
    cairo_surface_t *surface = mGerberViewer->getSurface();

    cv::Mat m(mGerberViewer->getWidth(), mGerberViewer->getHeight(), CV_8UC3);


    CairoToMat(surface, m);


    mCVViewer->showImage(m);
}

void ViewProcessor::update(GerberLayer &inGerberLayer)
{
    recalculateSize();

    mGerberViewer->drawLayer(inGerberLayer);
    cairo_surface_t *surface = mGerberViewer->getSurface();

    cv::Mat m(mGerberViewer->getWidth(), mGerberViewer->getHeight(), CV_8UC3);


    CairoToMat(surface, m);


    mCVViewer->showImage(m);
}

void ViewProcessor::recalculateSize()
{
    if(mCVViewer->getWidth() != mGerberViewer->getWidth() || mCVViewer->getHeight() != mGerberViewer->getHeight()){
        mGerberViewer->initCairo(mCVViewer->getWidth(), mCVViewer->getHeight());
    }
}
