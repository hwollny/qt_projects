#include "imageviewer.h"
#include "ui_imageviewer.h"


ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer),
    wi(0), image(0), dialog(new QDialog)
    {
    ui->setupUi(this);

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);


    openAct = ui->actionOpen;
    closeAct = ui->actionClose;
    saveAsAct = ui->actionSave_as;
    exitAct = ui->actionExit;
    zoomInAct = ui->actionZoom_In;
    zoomOutAct = ui->actionZoom_Out;
    normalSizeAct = ui->actionNormal_Size;
    fitToWindowAct = ui->actionFit_to_Window;
    brightContrAct = ui->actionBrightness_Contrast;


    setWindowTitle(tr("Image Viewer"));
    resize(500, 400);
}

ImageViewer::~ImageViewer()
{
    delete ui;
}
void ImageViewer::updateActions()
{

    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
    brightContrAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::on_actionOpen_triggered()
{
    qDebug() << "open()";
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        image = new QImage(fileName);
        if (image->isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        imageLabel->setPixmap(QPixmap::fromImage(*image));
        imageLabel->adjustSize();
        scaleFactor = 1.0;

        closeAct->setEnabled(true);
        saveAsAct->setEnabled(true);
        fitToWindowAct->setEnabled(true);
        updateActions();

        if (!fitToWindowAct->isChecked())
            imageLabel->adjustSize();

    }
}

void ImageViewer::on_actionClose_triggered()
{
  imageLabel->clear();
}

void ImageViewer::on_actionSave_as_triggered()
{
    while(true == true) {
        QString fileName = QFileDialog::getSaveFileName(this,tr("Save File As"), QDir::currentPath(),
                                                        tr("Images (*.png *.xpm *.jpg)") );
        if(fileName.isEmpty()) return;

        if(!imageLabel->pixmap()->save(fileName)) {
          if(yes_no_box(fileName)) break;
        }
        else break;
    }
}

bool ImageViewer::yes_no_box(const QString& filename)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Warning", "Unsupported file name " + filename +" You want to exit anyhow",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        return true;
    }
    return false;

}

void ImageViewer::on_actionFit_to_Window_triggered()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        on_actionNormal_Size_triggered();
    }
    updateActions();
}

void ImageViewer::on_actionZoom_In_triggered()
{
  scaleImage(1.25);
}

void ImageViewer::on_actionZoom_Out_triggered()
{
  scaleImage(0.8);
}

void ImageViewer::on_actionNormal_Size_triggered()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}
void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageViewer::on_actionBrightness_Contrast_triggered()
{

    if(wi==0) {
        wi = new Ui::PopUpImageSettings;
        wi->setupUi(dialog);
        dialog->show();
    }

    if(!dialog->isActiveWindow()) dialog->show();

    QObject::connect(wi->pushButton,  SIGNAL(pressed()),this,SLOT(on_actionBrCo_value_changed()));
    //QObject::connect(wi->doubleSpinBox_2,SIGNAL(editingFinished()),this,SLOT(on_actionBrCo_value_changed()));

 }

void ImageViewer::on_actionBrCo_value_changed()
{
    double beta = wi->doubleSpinBox->value();
    double alpha = wi->doubleSpinBox_2->value();

    //boxBrightContr->setEnabled(true);
    Q_ASSERT(image);
    cv::Mat cvMat = ASM::QPixmapToCvMat(QPixmap::fromImage(*image));
    cv::Mat cvMat_new = cv::Mat::zeros( cvMat.size(), cvMat.type() );

    //QDoubleSpinBox box(this);

    cvMat.convertTo(cvMat_new, -1, alpha, beta);
    imageLabel->setPixmap(ASM::cvMatToQPixmap(cvMat_new));
}

