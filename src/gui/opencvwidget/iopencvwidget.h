#ifndef IOPENCVWIDGET_H
#define IOPENCVWIDGET_H

#include <iostream>
#include <stdint.h>
using namespace std;


#include <opencv2/core/core.hpp>


/// This interface is dedicated to handle a Open CV context
class IOpenCVWidget{
    public:
        IOpenCVWidget() {}
        virtual ~IOpenCVWidget() {}

        virtual bool showImage(cv::Mat &inImage) = 0; /// Used to set the image to be viewed

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
};


#endif
