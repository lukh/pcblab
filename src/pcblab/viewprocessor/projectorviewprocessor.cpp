#include "projectorviewprocessor.h"

ProjectorViewProcessor::ProjectorViewProcessor(PcbLab *inPcb):
    IViewProcessor(inPcb), mCamera(NULL)
{
    mKOLayerName = "GKO";

    mCamera = new cv::VideoCapture(0); // open the default camera
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

void ProjectorViewProcessor::setBackgroundColor(plPoint inPoint)
{
    const int roi_size = 100;
    // set roi
    int x, y;
    x = inPoint.mX - roi_size/2;
    y = inPoint.mY - roi_size/2;
    if(x < 0) { x = 0; }
    else if(x > (mCurrentFrame.size().width - roi_size)) { x = mCurrentFrame.size().width - roi_size; }
    if(y < 0) { y = 0; }
    else if(y > (mCurrentFrame.size().height - roi_size)) { y = mCurrentFrame.size().height - roi_size; }
    cv::Rect roi = cv::Rect(x, y, roi_size, roi_size); // should be checked to avoid catched exception

    try{
        cv::Mat img_roi = mCurrentFrame(roi);
        cv::imshow("ROI", img_roi);

        mainColorExtraction(img_roi);

    }
    catch( cv::Exception& e ){
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
    }
}

cv::Mat ProjectorViewProcessor::process()
{
    if(!mCamera->isOpened()){  // check if we succeeded
        return cv::Mat();
    }

    mCamera->read(mCurrentFrame);

    extractPcbOutlineFromImage();

    return mCurrentFrame;
}

vector<plPoint> ProjectorViewProcessor::extractPcbOutlineFromGerber()
{
    return vector<plPoint>();
}

void ProjectorViewProcessor::projectorCalibration()
{

}

vector<plPoint> ProjectorViewProcessor::extractPcbOutlineFromImage()
{
    cv::Mat blur;
    cv::Size ks(7,7);
    cv::GaussianBlur(mCurrentFrame, blur, ks, 0);

    //convert image to hsv
    cv::Mat hsv;
    cv::cvtColor(blur, hsv, cv::COLOR_BGR2HSV);


    //define range of blue color in HSV
    uint32_t pcb_hue = 65;
    uint32_t pcb_hue_margin = 20;

    //Threshold the HSV image to get only blue colors
    cv::Mat color_mask;
    cv::inRange(hsv, cv::Scalar(pcb_hue-pcb_hue_margin,50,50), cv::Scalar(pcb_hue+pcb_hue_margin,230,230), color_mask);
    //imshow("Color Mask", color_mask);



    return vector<plPoint>();
}


