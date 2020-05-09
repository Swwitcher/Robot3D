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


//////////////////// OBJECT DETECTION ////////////////////////

/**
 * @brief return the grayscale value of the object on the disparity map
 * @param dispmap the disparity map to consider
 * @return return the grayscale value of the object on the disparity map
 */
int ImageProcessing::object_gs_value(cv::Mat dispmap){
    const double seuil = 2000
            ;
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

    for(int i=254; i>=0; i--){
        if(colorTab[i] >= seuil)
            return i;
    }

    return -1;
}

/**
 * @brief check if the object is to far or to close
 * @param base_value the base value to consider
 * @param dispmap the disparity map tyo consider
 * @return -1 if the object is too close, 1 if it's too far, 0 otherwise
 */
int ImageProcessing::forward_or_backward(int base_value, cv::Mat dispmap){
    int obj_gs = object_gs_value(dispmap);
    //error
    if(obj_gs == -1) return -2;
    //Good dist
    if(obj_gs == base_value) return 0;
    //Too close
    if(obj_gs > base_value) return -1;
    //Too far
    if(obj_gs < base_value) return 1;

    //error
    return -2;
}

/**
 * @brief try to find the object on the disparity map
 * @param dispmap the disparity map to consider
 * @return 1 if the object is on the right, -1 on the left, 0 center
 */
int ImageProcessing::left_or_right(cv::Mat dispmap){
    int seuil = 25;
    int obj_gs = object_gs_value(dispmap);
    int colsTab[dispmap.cols];

    for(int i=0; i<dispmap.cols; i++){
        colsTab[i] = 0;
    }
    int index;
    for(int y=0; y<dispmap.cols; y++){
        for(int x=0; x<dispmap.rows; x++){
            index = (int)dispmap.at<uchar>(x, y);
            if(index == obj_gs)
                colsTab[y]++;
        }
    }

    //Object centered
    if(colsTab[dispmap.cols/2] >= seuil) return 0;
    //fprintf(stderr, "TEST %d\n", colsTab[dispmap.cols/2]);
    int index_max_gs = -1;
    int tmp_max = 0;
    for(int i=0; i<dispmap.cols; i++){
        //fprintf(stderr, "ColsTab[%d] = %d\n", i, colsTab[i]);
        if(colsTab[i] > tmp_max){
            index_max_gs = i;
            tmp_max = colsTab[i];
        }
    }

    if(index_max_gs > dispmap.cols/2) return 1;
    else return -1;

}

