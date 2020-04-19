#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#include <opencv2/calib3d.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"

class ImageProcessing
{
public:
    ImageProcessing();
    int find_objet_grayscale(cv::Mat dispmap);
    int standard_color(cv::Mat dispmap);
    cv::Mat dispMap(cv::Mat left, cv::Mat right);
    int object_dist(cv::Mat dispmap);
    int object_deg(cv::Mat dispmap);
};

#endif // IMAGEPROCESSING_H
