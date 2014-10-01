#include "imageviewer.h"
#include "ui_imageviewer.h"


ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer),
    wi(0), imageLabel(0), scrollArea(0), image(0), ImageSettings(0), dialog(new QDialog), popup_linFilt(0), popup_sharpen(0), popup_binary(0)
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


    setWindowTitle(tr("Image Viewer"));
    resize(500, 400);
}

ImageViewer::~ImageViewer()
{
    qDebug() << "Call destructor of ImageViewer ";
    if(ui) { delete ui; ui = 0; }
    if(wi) { delete wi; wi = 0 ; }
    if(popup_linFilt) { popup_linFilt->close(); delete popup_linFilt; popup_linFilt = 0; }
    if(popup_sharpen) { popup_sharpen->close(); delete popup_sharpen; popup_sharpen = 0; }
    if(popup_binary) { popup_binary->close(); delete popup_binary; popup_binary = 0; }

}
void ImageViewer::updateActions()
{

    ui->actionZoom_In->setEnabled(!ui->actionFit_to_Window->isChecked());
    ui->actionZoom_Out->setEnabled(!ui->actionFit_to_Window->isChecked());
    ui->actionNormal_Size->setEnabled(!ui->actionFit_to_Window->isChecked());
    ui->actionBrightness_Contrast->setEnabled(true);
    ui->actionEqualization->setEnabled(true);
    ui->actionInverte->setEnabled(true);
    ui->actionLinear_Filter->setEnabled(true);
    ui->actionSharpen->setEnabled(true);
    ui->actionConnected_Areas->setEnabled(true);
    ui->actionMake_Binary->setEnabled(true);
    ui->actionDFT->setEnabled(true);
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

        ui->actionClose->setEnabled(true);
        ui->actionSave_as->setEnabled(true);
        ui->actionFit_to_Window->setEnabled(true);
        updateActions();

        if (!ui->actionFit_to_Window->isChecked())
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
    bool fitToWindow = ui->actionFit_to_Window->isChecked();
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

    ui->actionZoom_In->setEnabled(scaleFactor < 3.0);
    ui->actionZoom_Out->setEnabled(scaleFactor > 0.333);
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

    QObject::connect(popup_linFilt->GetPushButton(), SIGNAL(pressed()),this,SLOT(on_actionLinear_Filter_triggered_t()));

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

    QObject::connect(popup_sharpen->GetPushButton(), SIGNAL(pressed()),this,SLOT(on_actionSharpen_triggered_t()));

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

void ImageViewer::on_actionConnected_Areas_triggered()
{
    cv::Mat cvMat = ASM::QPixmapToCvMat(QPixmap::fromImage(*image));
    cv::Mat output = cv::Mat::zeros(cvMat.size(), CV_8UC3);

    output.setTo(cv::Scalar(255,255,255));
    cv::Mat grayscaleMat (cvMat.size(), CV_8U);

        //Convert BGR to Gray
    cv::cvtColor( cvMat, grayscaleMat, CV_BGR2GRAY );

        //Binary image
    cv::Mat binary(grayscaleMat.size(), grayscaleMat.type());


    cv::threshold(grayscaleMat, binary, 160.0, 255.0, cv::THRESH_BINARY);


    std::vector < std::vector<cv::Point2i > > blobs;
    FindBlobs(binary, blobs);

    for(size_t i=0; i < blobs.size(); i++) {
            unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
            unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
            unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));

            for(size_t j=0; j < blobs[i].size(); j++) {
                int x = blobs[i][j].x;
                int y = blobs[i][j].y;

                output.at<cv::Vec3b>(y,x)[0] = b;
                output.at<cv::Vec3b>(y,x)[1] = g;
                output.at<cv::Vec3b>(y,x)[2] = r;
            }
        }
    qDebug() << "#connected areas:  " << blobs.size();

    imageLabel->setPixmap(ASM::cvMatToQPixmap(output).copy());
}

void ImageViewer::on_actionMake_Binary_triggered()
{
    if(!popup_binary) {
        popup_binary = new QSpinBoxPopUp(NULL,1,"Make Binary");
        popup_binary->SetMaxValueBox(0,255);
        popup_binary->SetValueBox(0,120);
    }
    popup_binary->show();

    QObject::connect(popup_binary->GetPushButton(), SIGNAL(pressed()),this,SLOT(on_actionMake_Binary_triggered_t()));
}

void ImageViewer::on_actionMake_Binary_triggered_t()
{
    cv::Mat cvMat = ASM::QPixmapToCvMat(QPixmap::fromImage(*image));
    cv::Mat grayscaleMat (cvMat.size(), CV_8U);
    cv::cvtColor( cvMat, grayscaleMat, CV_BGR2GRAY );
    cv::Mat binary(grayscaleMat.size(), grayscaleMat.type());

    cv::threshold(grayscaleMat, binary, popup_binary->GetValueBox(0), 255.0, cv::THRESH_BINARY);

    imageLabel->setPixmap(ASM::cvMatToQPixmap(binary).copy());
}

void ImageViewer::on_actionExit_triggered()
{
    this->close();
}

 void ImageViewer::closeEvent(QCloseEvent *c)
 {
qDebug() << "Close Event called";
if(wi) {dialog->close(); }
if(popup_linFilt) { popup_linFilt->close(); }
if(popup_sharpen) { popup_sharpen->close(); }
if(popup_binary) { popup_binary->close(); }
 }


void ImageViewer::on_actionDFT_triggered()
{
   cv::Mat cvMat = ASM::QPixmapToCvMat(QPixmap::fromImage(*image));
   cv::Mat grayscaleMat (cvMat.size(), CV_8U);
   cv::cvtColor( cvMat, grayscaleMat, CV_BGR2GRAY );
   cv::Mat cvMat_dft =  computeDFT(grayscaleMat);
   cv::Mat magI;
   updateMag(cvMat_dft,magI);
   //cv::Mat cvMat_dft_filtered;
   //magI.convertTo(show,CV_8U);

   //cv::Mat box = cv::Mat::ones(cvMat_dft.size(), cvMat_dft.type());
   //createBoxMask(box,box.cols/1,box.rows/1);
   //cvMat_dft_filtered = box.mul(cvMat_dft);

   //Mat mask = createGausFilterMask(cvMat_dft.size(), box.rows/2, box.cols/2, 100, true, true);
   Mat mask = createGausFilterMask(cvMat_dft.size(), cvMat_dft.rows/2, cvMat_dft.cols/2, min(cvMat_dft.rows,cvMat_dft.cols), true, true);

   //shift(mask);

   Mat planes[] = {Mat::zeros(cvMat_dft.size(), CV_32F), Mat::zeros(cvMat_dft.size(), CV_32F)};
   Mat kernel_spec;
   planes[0] = mask; // real
   planes[1] = mask; // imaginar
   imshow("mask", planes[0]);
   merge(planes, 2, kernel_spec);

   mulSpectrums(cvMat_dft, kernel_spec, cvMat_dft , DFT_ROWS); // only DFT_ROWS accepted
   cv::Mat magI2;
   updateMag(cvMat_dft,magI2);		// show spectrum


   cv::Mat inverseTransform;
   cv::dft(cvMat_dft,inverseTransform,cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
    normalize(inverseTransform, inverseTransform, 0, 1, CV_MINMAX);
   // Back to 8-bits
   //cv::Mat finalImage;
  // inverseTransform.convertTo(finalImage, CV_8U);

//imshow("box", box);
imshow("spectrum", magI);
imshow("spectrum filtered", magI2);
imshow("filtered", inverseTransform);
   QWidget wdg1(this);
   QLabel *imageLabel1 = new QLabel;
   imageLabel1->setBackgroundRole(QPalette::Base);
   imageLabel1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
   imageLabel1->setScaledContents(true);

   QScrollArea *scrollArea1 = new QScrollArea;
   scrollArea1->setBackgroundRole(QPalette::Dark);
   scrollArea1->setWidget(imageLabel1);


   //wdg1.show();
   //imageLabel1->setPixmap(ASM::cvMatToQPixmap(show).copy());
   //imageLabel1->adjustSize();
}

