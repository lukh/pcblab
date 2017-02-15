#include "imageprocessing.h"



HSColor mainColorExtraction(const cv::Mat &inHsvImage, int hbins, int sbins)
{

    cv::Mat hist;


    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = {0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };

    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    cv::calcHist(&inHsvImage, 1, channels, cv::Mat(), // do not use mask
             hist, 2, histSize, ranges,
             true, // the histogram is uniform
             false );

    imshow("source", inHsvImage);
    imshow("Hist", hist);


    double min, max;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(hist, &min, &max, &min_loc, &max_loc);

    return HSColor(180*max_loc.y/hbins, 256*max_loc.x/sbins);
}
