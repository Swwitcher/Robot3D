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
    cv::Mat dispMap(cv::Mat left, cv::Mat right);
    int object_gs_value(cv::Mat dispmap);
    int left_or_right(cv::Mat dispmap);
    int forward_or_backward(int base_value, cv::Mat dispmap);
    ~MainWindow();
};

#endif // MAINWINDOW_H
