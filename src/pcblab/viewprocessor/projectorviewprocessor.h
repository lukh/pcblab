#ifndef PROJECTORVIEWPROCESSOR_H
#define PROJECTORVIEWPROCESSOR_H

#include <vector>
#include <string>


#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "iviewprocessor.h"
#include "iopencvwidget.h"

using namespace std;

using namespace cv;

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

        // -------------- Runtime ---------------

        /// Display the Cairo's image on the OpenCV Surface, with the perspective correction
        Mat process();



    private:
        // ------------ Initialization -----------

        /// Extract outline from the gerber, in renderer image coordinates.
        vector <plPoint> extractPcbOutlineFromGerber();

        void projectorCalibration();

        // -------------- Runtime ---------------

        /// Extract PCB outline from the image provided by the webcam
        /// return the list of points found, in the real image coordinates (px)
        vector <plPoint> extractPcbOutlineFromImage(Mat &inImg);

    private:
        // camera
        cv::VideoCapture *mCamera; // open the default camera


        // gerbers extraction info
        string mKOLayerName;

        // image extraction parameters

        // current camera transformation, computed by extractPcbOutlineFromImage
        /// Transformation to project Gerber into Camera Space


        // current projector transformation
        /// Transformation to project Gerber to Projector (???)

};

#endif // PROJECTORVIEWPROCESSOR_H
