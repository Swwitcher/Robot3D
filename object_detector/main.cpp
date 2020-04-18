#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //First set
    cv::Mat left1 = cv::imread("/home/pierre/Bureau/S6/Projet_Tech/img_projet/set2/left1.png");
    cv::Mat right1 = cv::imread("/home/pierre/Bureau/S6/Projet_Tech/img_projet/set2/right1.png");

    //Second set
    cv::Mat left2 = cv::imread("/home/pierre/Bureau/S6/Projet_Tech/img_projet/set1/left1.png");
    cv::Mat right2 = cv::imread("/home/pierre/Bureau/S6/Projet_Tech/img_projet/set1/right1.png");

    //Third set
    cv::Mat left3 = cv::imread("/home/pierre/Bureau/S6/Projet_Tech/img_projet/set3/left1.png");
    cv::Mat right3 = cv::imread("/home/pierre/Bureau/S6/Projet_Tech/img_projet/set3/right1.png");

    //Disp
    cv::Mat disp1 = w.dispMap(left1, right1);
    cv::Mat disp2 = w.dispMap(left2, right2);
    cv::Mat disp3 = w.dispMap(left3, right3);

    cv::imshow("1", disp1);
    cv::imshow("2", disp2);
    cv::imshow("3", disp3);

    fprintf(stderr, "Etalon 1 : %d\n", w.standard_color(disp1));
    fprintf(stderr, "Etalon 2 : %d\n", w.standard_color(disp2));
    fprintf(stderr, "Etalon 3 : %d\n", w.standard_color(disp3));

    return a.exec();
}
