#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <iostream>
#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QImageWriter>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QRadioButton>
#include "qt_to_opencv_converter.h"
#include "asm_converter.h"
#include "ui_popupimagesettings.h"
#include "../pic_equalization.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "popup_window1.h"
#include "qspinboxpopup.h"
#include "qlabelpopup.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>


namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);

    ~ImageViewer();


    //add a comment
private slots:
    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_actionSave_as_triggered();

    void on_actionFit_to_Window_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionNormal_Size_triggered();

    bool yes_no_box(const QString& filename);

    void on_actionBrightness_Contrast_triggered();

    void on_actionBrCo_value_changed();

    void on_actionEqualization_triggered();

    void on_actionInverte_triggered();

    void on_actionLinear_Filter_triggered();

    void on_actionLinear_Filter_triggered_t();

    void on_actionSharpen_triggered();

    void on_actionSharpen_triggered_t();

    void on_actionConnected_Areas_triggered();

    void on_actionMake_Binary_triggered();

    void on_actionMake_Binary_triggered_t();

    void on_actionExit_triggered();

    void on_actionDFT_triggered();

private:
    Ui::ImageViewer *ui;
    Ui::PopUpImageSettings *wi;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QImage *image;
    //cv::Mat *cvMat;


    QDialog *dialog;


    popup_window1 *popup_linFilt;
    popup_window1 *popup_sharpen;
    QSpinBoxPopUp *popup_binary;

    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *item;

    double scaleFactor;
    double alpha;
    double beta;

    void updateActions();

    void scaleImage(double factor);

    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    void closeEvent(QCloseEvent *c);
};

#endif // IMAGEVIEWER_H
