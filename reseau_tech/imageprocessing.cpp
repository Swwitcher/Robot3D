#include "imageprocessing.h"

ImageProcessing::ImageProcessing()
{

}

cv::Mat ImageProcessing::dispMap(cv::Mat left, cv::Mat right){
    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(0, 16, 10);
    cv::Mat cv_res, disp;
    //
    sgbm->setBlockSize(5);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(64);
    sgbm->setDisp12MaxDiff(-1);
    sgbm->setSpeckleRange(93);
    sgbm->setSpeckleWindowSize(921);
    sgbm->setP1(0);
    sgbm->setP2(694);
    sgbm->setPreFilterCap(1);
    sgbm->setUniquenessRatio(34);
    //
    sgbm->setMode(cv::StereoSGBM::MODE_SGBM_3WAY);
    sgbm->compute(left, right, cv_res);
    cv::normalize(cv_res, disp, 0, 255, CV_MINMAX, CV_8U);
    return(disp);
}

int ImageProcessing::standard_color(cv::Mat dispmap){
    const double seuil = 1000;
    int colorTab[255];

    //Init tab
    for(int i=0; i<255; i++){
        colorTab[i] = 0;
    }
    int index;
    //Look for the number of pixels per grayscale in the map = the object
    for(int x=0; x<dispmap.rows; x++){
        for(int y=0; y<dispmap.cols; y++){
            index = (int)dispmap.at<uchar>(x, y);
            colorTab[index] ++;
        }
    }

    int res = 0;
    for(int i=254; i>=0; i--){
        if(colorTab[i] >= seuil)
            return i;
    }

    return 0;
}
int ImageProcessing::object_dist(cv::Mat dispmap){
    return 0;
}

int ImageProcessing::object_deg(cv::Mat dispmap){
    return 0;
}

