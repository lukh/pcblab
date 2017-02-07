#include "projectorviewprocessor.h"

ProjectorViewProcessor::ProjectorViewProcessor(PcbLab *inPcb):
    IViewProcessor(inPcb)
{
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

}

plPoint ProjectorViewProcessor::convertCoordsFromImageToReal(plPoint inImgCoords)
{
    return plPoint();
}

void ProjectorViewProcessor::show(Mat &outImage)
{
    if(!mCamera->isOpened()){  // check if we succeeded
        return;
    }


    mCamera->read(outImage);
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
    return vector<plPoint>();
}


