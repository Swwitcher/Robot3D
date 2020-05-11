#include "imageprocessing.h"

ImageProcessing::ImageProcessing()
{

}

cv::Mat ImageProcessing::dispMap(cv::Mat left, cv::Mat right){
    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(0, 16, 10);
    cv::Mat cv_res, disp;
    //
    sgbm->setBlockSize(21);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(16);
    sgbm->setDisp12MaxDiff(21);
    sgbm->setSpeckleRange(0);
    sgbm->setSpeckleWindowSize(0);
    sgbm->setP1(2048);
    sgbm->setP2(0);
    sgbm->setPreFilterCap(1);
    sgbm->setUniquenessRatio(0);
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
    const double seuil = 2000;
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
 * @brief Return the number of pixel of the object on the dispmap
 * @param dispmap the disparity map to consider
 * @return the number of pixels of the object
 */
int ImageProcessing::obj_pix_nb(cv::Mat dispmap){

    int approx = 0;
    int obj_gs = object_gs_value(dispmap);
    int res = 0, pix_gs;
    for(int x=0; x<dispmap.rows; x++){
        for(int y=0; y<dispmap.cols; y++){
            pix_gs = (int)dispmap.at<uchar>(x, y);
            if(pix_gs >=obj_gs-approx && pix_gs <=obj_gs+approx)
                res++;
        }
    }
    return res;

    //Other algorithm, look for the width of the object
    /*
    int pix_gs;
    int seuil = 20;
    int gs_value = object_gs_value(dispmap);
    int tab[dispmap.rows];
    for(int x=0; x<dispmap.rows; x++){
        for(int y=0; y<dispmap.cols; y++){
            pix_gs = (int)dispmap.at<uchar>(x, y);
            if(pix_gs == gs_value)
                tab[x]++;
        }
    }
    std::sort(tab, tab+480);
    int tmp = 0;
    for(int i=0; i<480; i++)
        if(tab[i] > seuil){
            tmp = i;
            break;
        }

    printf("MEDIANE = %d\n", tab[240]);


    return tab[(480-tmp)/2];
    */
}

/**
 * @brief check if the object is to far or to close
 * @param base_value the base value to consider
 * @param dispmap the disparity map tyo consider
 * @return -1 if the object is too close, 1 if it's too far, 0 otherwise
 */
int ImageProcessing::forward_or_backward(int base_value, cv::Mat dispmap){
    int approx = 0;
    //int obj_gs = object_gs_value(dispmap);
    int obj_gs = obj_pix_nb(dispmap);
    //error
    if(obj_gs == -1) return -2;
    //Good dist
    if(obj_gs > base_value-approx && obj_gs < base_value+approx) return 0;
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

    if(index_max_gs > dispmap.cols/2) return 2;
    else return 3;

}

