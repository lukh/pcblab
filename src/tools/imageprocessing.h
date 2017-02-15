#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

///Handles Hue/Sat color
class HSColor{
    public:
        HSColor(): mHue(0), mSat(0) {}
        HSColor(uint8_t inHue, uint8_t inSat): mHue(inHue), mSat(inSat) {}

        uint8_t mHue;
        uint8_t mSat;
};

HSColor mainColorExtraction(const cv::Mat &inHsvImage, int hbins = 60, int sbins = 32);


#endif // IMAGEPROCESSING_H
