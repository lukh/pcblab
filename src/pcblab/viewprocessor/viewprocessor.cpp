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





ViewProcessor::ViewProcessor(IOpenCVViewer *inCVViewer, CairoGerberViewer *inGerberView):
    mCVViewer(inCVViewer), mGerberViewer(inGerberView)
{

}

void ViewProcessor::update(const GerberLayer &inGerberLayer)
{
    mGerberViewer->drawLayer(inGerberLayer);
    cairo_surface_t *surface = mGerberViewer->getSurface();

    cv::Mat m;


    CairoToMat(surface, m);


    mCVViewer->showImage(m);
}
