#ifndef IOPENCVVIEWERGL_H
#define IOPENCVVIEWERGL_H

#include <iostream>
#include <stdint.h>
using namespace std;

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_0>
#include <opencv2/core/core.hpp>


/// This interface is dedicated to handle a Open CV context
class IOpenCVViewer{
    public:
        IOpenCVViewer() {}
        virtual ~IOpenCVViewer() {};

        virtual bool showImage(cv::Mat &inImage) = 0; /// Used to set the image to be viewed

        virtual uint32_t getWidth() = 0;
        virtual uint32_t getHeight() = 0;
};


#endif
