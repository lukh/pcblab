#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


void histogram(const cv::Mat &inHsvImage, cv::Mat &outHist, int hbins = 30, int sbins = 32);

cv::Scalar mainColorExtraction(const cv::Mat &inHsvImage, int hbins = 30, int sbins = 32);


#endif // IMAGEPROCESSING_H
