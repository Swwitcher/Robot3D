#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <opencv2/calib3d.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    int find_objet_grayscale(cv::Mat dispmap);
    cv::Mat dispMap(cv::Mat left, cv::Mat right);
    int object_dist(cv::Mat dispmap);
    int object_deg(cv::Mat dispmap);
    ~MainWindow();
};

#endif // MAINWINDOW_H
