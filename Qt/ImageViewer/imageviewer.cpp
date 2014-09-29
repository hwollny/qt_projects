#include "imageviewer.h"
#include "ui_imageviewer.h"


ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer),
    wi(0), image(0), dialog(new QDialog), popup_linFilt(0), popup_sharpen(0)
    {
    ui->setupUi(this);
    image  = 0;
    //cvMat = 0;
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
    equalAct = ui->actionEqualization;
    invAct = ui->actionInverte;
    linFiltAct = ui->actionLinear_Filter;
    sharpAct = ui->actionSharpen;


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
    brightContrAct->setEnabled(true);
    equalAct->setEnabled(true);
    invAct->setEnabled(true);
    linFiltAct->setEnabled(true);
    sharpAct->setEnabled(true);
}

void ImageViewer::on_actionOpen_triggered()
{
    qDebug() << "open()";
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        if(image) { delete image; image = 0; }
       //if(cvMat) { delete cvMat; cvMat = 0; }

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
    qDebug() << "ImageViewer::on_actionBrCo_value_changed()  begin";
    double beta = wi->doubleSpinBox->value();
    double alpha = wi->doubleSpinBox_2->value();
    if(beta!=0.0 || alpha!=1.0) {
        //boxBrightContr->setEnabled(true);
        if(wi->radioButton->isChecked()) {
            Q_ASSERT(image);
            qDebug() << "ImageViewer::on_actionBrCo_value_changed()  converte image to cv";
            cv::Mat cvMat = ASM::QPixmapToCvMat(QPixmap::fromImage(*image));
            qDebug() << "ImageViewer::on_actionBrCo_value_changed()  create empty picture";
            cv::Mat cvMat_new = cv::Mat::zeros( cvMat.size(), cvMat.type() );

            //QDoubleSpinBox box(this);

            qDebug() << "ImageViewer::on_actionBrCo_value_changed()  Apply chnages";
            cvMat.convertTo(cvMat_new, -1, alpha, beta);
            qDebug() << "ImageViewer::on_actionBrCo_value_changed()  Converte back to pixmap";
            imageLabel->setPixmap(ASM::cvMatToQPixmap(cvMat_new).copy());
        }
        else {
            Q_ASSERT(*imageLabel->pixmap());
            qDebug() << "ImageViewer::on_actionBrCo_value_changed()  converte image to cv";
            cv::Mat cvMat = ASM::QPixmapToCvMat(*imageLabel->pixmap());
            qDebug() << "ImageViewer::on_actionBrCo_value_changed()  create empty picture";
            cv::Mat cvMat_new = cv::Mat::zeros( cvMat.size(), cvMat.type() );
            qDebug() << "ImageViewer::on_actionBrCo_value_changed()  Apply chnages";
            cvMat.convertTo(cvMat_new, -1, alpha, beta);
            qDebug() << "ImageViewer::on_actionBrCo_value_changed()  Converte back to pixmap";
            imageLabel->setPixmap(ASM::cvMatToQPixmap(cvMat_new).copy());
        }
    }
    else if(wi->radioButton->isChecked()) imageLabel->setPixmap(QPixmap::fromImage(*image));
}



void ImageViewer::on_actionEqualization_triggered()
{
    test(ASM::QPixmapToCvMat(*imageLabel->pixmap()));
}

void ImageViewer::on_actionInverte_triggered()
{
    cv::Mat  mat = ASM::QPixmapToCvMat(*imageLabel->pixmap());
    inverte(mat);
    imageLabel->setPixmap(ASM::cvMatToQPixmap(mat).copy());
}

void ImageViewer::on_actionLinear_Filter_triggered()
{
    if(!popup_linFilt) {
            popup_linFilt = new popup_window1(NULL,"Kernel Size","Anchor Point");
        popup_linFilt->setWindowTitle("Linear Filter");
    }

    popup_linFilt->show();

    QObject::connect(popup_linFilt->btn1, SIGNAL(pressed()),this,SLOT(on_actionLinear_Filter_triggered_t()));

}
void ImageViewer::on_actionLinear_Filter_triggered_t()
{
    int kernel_size = popup_linFilt->GetValueBox1();
    int anchor_xy = popup_linFilt->GetValueBox2();

    if(anchor_xy>kernel_size-1) {
        anchor_xy=kernel_size-1;
        popup_linFilt->SetValueBox2(anchor_xy);
     }

    cv::Mat kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    cv::Mat dst;
    cv::Point anchor;
    anchor = cv::Point( anchor_xy, anchor_xy );
    double delta = 0.0;
    int ddepth = -1;
      /// Apply filter
    filter2D(ASM::QPixmapToCvMat(QPixmap::fromImage(*image)), dst, ddepth , kernel, anchor, delta, cv::BORDER_DEFAULT );

    imageLabel->setPixmap(ASM::cvMatToQPixmap(dst).copy());
}

void ImageViewer::on_actionSharpen_triggered()
{
    qDebug() << "sharpen()";
    if(!popup_sharpen) {
            popup_sharpen = new popup_window1(NULL,"Kernel Size","Sigma");
        popup_sharpen->setWindowTitle("Sharpen");

        popup_sharpen->SetStepSizeBox1(2);
    }

    popup_sharpen->show();

    QObject::connect(popup_sharpen->btn1, SIGNAL(pressed()),this,SLOT(on_actionSharpen_triggered_t()));

}

void ImageViewer::on_actionSharpen_triggered_t()
{
    int kernel_size = popup_sharpen->GetValueBox1();
    double sigma = popup_sharpen->GetValueBox2();

    cv::Mat cvMat = ASM::QPixmapToCvMat(QPixmap::fromImage(*image));
    cv::Mat cvMat_new = cv::Mat::zeros( cvMat.size(), cvMat.type() );


    cv::GaussianBlur(cvMat, cvMat_new, cv::Size(kernel_size, kernel_size), sigma);
    cv::addWeighted(cvMat, 1.5, cvMat_new, -0.5, 0, cvMat_new);

    imageLabel->setPixmap(ASM::cvMatToQPixmap(cvMat_new).copy());
}
