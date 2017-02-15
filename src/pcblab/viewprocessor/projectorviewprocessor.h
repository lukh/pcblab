#ifndef PROJECTORVIEWPROCESSOR_H
#define PROJECTORVIEWPROCESSOR_H

#include <vector>
#include <string>


#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "tools/imageprocessing.h"

#include "iviewprocessor.h"
#include "iopencvwidget.h"

using namespace std;

class ColorExtractionSettings{
    public:
        ColorExtractionSettings(): mHueMargin(25), mSatMargin(60), mValLow(20), mValHigh(200) {}

        uint8_t mHueMargin;
        uint8_t mSatMargin;

        uint8_t mValLow;
        uint8_t mValHigh;
};

class ProjectorViewProcessor: public IViewProcessor
{
    public:
        ProjectorViewProcessor(PcbLab *inPcb);
        virtual ~ProjectorViewProcessor();

        void init(uint32_t inWidth, uint32_t inHeight);

        //<<< --- Implementation of IViewProcessor
        /// repaint all the surface
        virtual void refresh();

        /// convert coords from image (pixels) to reality (mm/in)
        virtual plPoint convertCoordsFromImageToReal(plPoint inImgCoords);
        // --- >>>



        // ------------ Initialization -----------
        /// initialize the background color by color extraction from the point (camera's coordinates)
        void setBackgroundColor(plPoint inPoint);

        // -------------- Runtime ---------------

        /// Display the Cairo's image on the OpenCV Surface, with the perspective correction
        cv::Mat process();



    private:
        // ------------ Initialization -----------

        /// Extract outline from the gerber, in renderer image coordinates.
        vector <plPoint> extractPcbOutlineFromGerber();

        void projectorCalibration();

        // -------------- Runtime ---------------

        /// Extract PCB outline from the image provided by the webcam
        /// return the list of points found, in the real image coordinates (px)
        vector <plPoint> extractPcbOutlineFromImage();

    private:
        // camera
        cv::VideoCapture *mCamera; // open the default camera

        // keep the last image frame from camera for analysis
        cv::Mat mCurrentFrame;


        // gerbers extraction info
        string mKOLayerName;

        // --- image extraction parameters ---
        //background
        ColorExtractionSettings mCESBackground;
        HSColor mBackgroundColor;

        // current camera transformation, computed by extractPcbOutlineFromImage
        /// Transformation to project Gerber into Camera Space


        // current projector transformation
        /// Transformation to project Gerber to Projector (???)

};

#endif // PROJECTORVIEWPROCESSOR_H
