#include "imageprocessing.h"

void histogram(const cv::Mat &inHsvImage, cv::Mat &outHist, int hbins, int sbins)
{
    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    cv::MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    cv::calcHist(&inHsvImage, 1, channels, cv::Mat(), // do not use mask
             hist, 2, histSize, ranges,
             true, // the histogram is uniform
             false );

    double maxVal=0;
    cv::minMaxLoc(hist, 0, &maxVal, 0, 0);

    outHist = cv::Mat::zeros(sbins, hbins, CV_8UC1);

    for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);

            cv::rectangle(outHist, cv::Point(h, s),
                        cv::Point( (h+1) - 1, (s+1) - 1),
                        cv::Scalar::all(intensity),
                        CV_FILLED );
        }
    cv::imshow("Hist", outHist);
}

cv::Scalar mainColorExtraction(const cv::Mat &inHsvImage, int hbins, int sbins)
{
    cv::Mat hist;
    histogram(inHsvImage, hist);

    double min, max;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(hist, &min, &max, &min_loc, &max_loc);

    return cv::Scalar((179 * max_loc.x)/hbins, (255 * max_loc.y)/sbins);
}
