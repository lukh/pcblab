#include "projectorviewprocessor.h"

ProjectorViewProcessor::ProjectorViewProcessor(PcbLab *inPcb):
    IViewProcessor(inPcb), mCamera(NULL)
{
    mKOLayerName = "GKO";

    mCamera = new VideoCapture(0); // open the default camera
    if(!mCamera->isOpened()){  // check if we succeeded

    }
}

ProjectorViewProcessor::~ProjectorViewProcessor()
{
    delete mCamera;
}

void ProjectorViewProcessor::init(uint32_t inWidth, uint32_t inHeight)
{
    // /////////////////////////////
    // init the viewer
    // /////////////////////////////
    setup(inWidth, inHeight);
}

void ProjectorViewProcessor::refresh()
{
    mViewer.clean();
    mGerberRenderer.drawAll(mPcb->getGerber());
    mExcellonRenderer.draw(mPcb->getExcellon());
    mComponentRenderer.draw(mPcb->getComponents());
    mNetlistRenderer.draw(mPcb->getNetlist());
}

plPoint ProjectorViewProcessor::convertCoordsFromImageToReal(plPoint inImgCoords)
{
    return plPoint();
}

Mat ProjectorViewProcessor::process()
{
    if(!mCamera->isOpened()){  // check if we succeeded
        return Mat();
    }
    Mat img;

    mCamera->read(img);

    extractPcbOutlineFromImage(img);

    return img;
}

vector<plPoint> ProjectorViewProcessor::extractPcbOutlineFromGerber()
{
    return vector<plPoint>();
}

void ProjectorViewProcessor::projectorCalibration()
{

}

vector<plPoint> ProjectorViewProcessor::extractPcbOutlineFromImage(Mat &inImg)
{
    Mat blur;
    Size ks(7,7);
    GaussianBlur(inImg, blur, ks, 0);

    //convert image to hsv
    Mat hsv;
    cvtColor(blur, hsv, COLOR_BGR2HSV);


    //define range of blue color in HSV
    uint32_t pcb_hue = 65;
    uint32_t pcb_hue_margin = 20;

    //Threshold the HSV image to get only blue colors
    Mat color_mask;
    inRange(hsv, Scalar(pcb_hue-pcb_hue_margin,50,50), Scalar(pcb_hue+pcb_hue_margin,230,230), color_mask);
    imshow("Color Mask", color_mask);



    return vector<plPoint>();
}


