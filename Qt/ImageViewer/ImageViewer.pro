#-------------------------------------------------
#
# Project created by QtCreator 2014-09-23T14:15:52
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer
TEMPLATE = app

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}
#LIBS += -lopencv_highgui
INCLUDEPATH += /usr/include/opencv/
INCLUDEPATH += /usr/include/opencv2/

SOURCES += main.cpp\
        imageviewer.cpp \
    qt_to_opencv_converter.cpp \
    popupimagesettings.cpp \
    ../pic_equalization.cpp

HEADERS  += imageviewer.h \
    qt_to_opencv_converter.h \
    asm_converter.h \
    popupimagesettings.h \
    ../pic_equalization.h

FORMS    += imageviewer.ui \
    popupimagesettings.ui
