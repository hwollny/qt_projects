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
INCLUDEPATH += /usr/local/include/
INCLUDEPATH += /usr/local/include/opencv/
INCLUDEPATH += /usr/local/include/opencv2/

SOURCES += main.cpp\
        imageviewer.cpp \
    qt_to_opencv_converter.cpp \
    popupimagesettings.cpp \
    ../pic_equalization.cpp \
    popup_window1.cpp \
    qspinboxpopup.cpp \
    qlabelpopup.cpp

HEADERS  += imageviewer.h \
    qt_to_opencv_converter.h \
    asm_converter.h \
    popupimagesettings.h \
    ../pic_equalization.h \
    popup_window1.h \
    qspinboxpopup.h \
    qlabelpopup.h

FORMS    += imageviewer.ui \
    popupimagesettings.ui
