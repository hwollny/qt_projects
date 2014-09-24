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

#include "qt_to_opencv_converter.h"
#include "asm_converter.h"
#include "ui_popupimagesettings.h"

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();

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

private:
    Ui::ImageViewer *ui;
    Ui::PopUpImageSettings *wi;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QImage *image;

    QAction *openAct;
    QAction *closeAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *brightContrAct;

    QWidget *ImageSettings;
    QDialog *dialog;

    double scaleFactor;
    double alpha;
    double beta;

    void updateActions();

    void scaleImage(double factor);

    void adjustScrollBar(QScrollBar *scrollBar, double factor);
};

#endif // IMAGEVIEWER_H
