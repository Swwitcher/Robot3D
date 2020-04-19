TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L /opt/local.lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_calib3d -lopencv_stereo
INCLUDEPATH += -I /opt/local/stow/opencv-3.4.2 -I/usr/local/include

SOURCES += main.cpp \
    imageprocessing.cpp

HEADERS += \
    imageprocessing.h
