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
    cv::Mat dispMap(cv::Mat left, cv::Mat right);
    int object_gs_value(cv::Mat dispmap);
    int left_or_right(cv::Mat dispmap);
    int forward_or_backward(int base_value, cv::Mat dispmap);

    int obj_pix_nb(cv::Mat dispmap);
};

#endif // IMAGEPROCESSING_H
