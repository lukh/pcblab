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
    return vector<plPoint>();
}


