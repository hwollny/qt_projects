#include "pic_equalization.h"

using namespace cv;

void test(cv::Mat image)
{
    /// Separate the image in 3 places ( B, G and R )
    std::vector<cv::Mat> bgr_planes;
    cv::split( image, bgr_planes );

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    cv::Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    cv::calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
              cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              cv::Scalar( 255, 0, 0), 2, 8, 0  );
        cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
              cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              cv::Scalar( 0, 255, 0), 2, 8, 0  );
        cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
              cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              cv::Scalar( 0, 0, 255), 2, 8, 0  );
    }

    /// Display
    cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE );
    cv::imshow("Histogram", histImage );

}



void inverte(cv::Mat& image)
{
      IplImage img = image;
      // get the image data
      int height    = (&img)->height;
      int width     = (&img)->width;
      int step      = (&img)->widthStep;
      int channels  = (&img)->nChannels;
      uchar *data      = (uchar *)(&img)->imageData;

      // invert the image
      for(int i=0;i<height;i++)
         for(int j=0;j<width;j++)
            for(int k=0;k<channels;k++)  //loop to read for each channel
               data[i*step+j*channels+k]=255-data[i*step+j*channels+k];    //inverting the image

        return;
}


void FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs)
{
    blobs.clear();

    // Fill the label_image with the blobs
    // 0  - background
    // 1  - unlabelled foreground
    // 2+ - labelled foreground

    cv::Mat label_image;
    binary.convertTo(label_image, CV_32SC1);

    double label_count = 2.; // starts at 2 because 0,1 are used already

    for(int y=0; y < label_image.rows; y++) {
        int *row = (int*)label_image.ptr(y);
        for(int x=0; x < label_image.cols; x++) {

            if(row[x] != 0) {
                continue;
            }

            cv::Rect rect;
            cv::floodFill(label_image, cv::Point(x,y),  label_count, &rect, 0., 0., 4);
           // qDebug () << "found connected area  in " << rect.y+rect.height << "  " << rect.x+rect.width;
            std::vector <cv::Point2i> blob;

            for(int i=rect.y; i < (rect.y+rect.height); i++) {
                int *row2 = (int*)label_image.ptr(i);
                for(int j=rect.x; j < (rect.x+rect.width); j++) {
                    if(row2[j] != label_count) {
                        continue;
                    }

                    blob.push_back(cv::Point2i(j,i));
                }
            }

            blobs.push_back(blob);

            label_count++;
        }
    }
}

void shift(Mat &magI) {

    // crop if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);                     // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void updateMag(Mat complex, Mat& magI)
{

    Mat planes[] = {Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F)};
    split(complex, planes);                // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

    magnitude(planes[0], planes[1], magI);    // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)

    // switch to logarithmic scale: log(1 + magnitude)
    magI += Scalar::all(1);
    log(magI, magI);

    shift(magI);
    normalize(magI, magI, 1, 0, NORM_INF); // Transform the matrix with float values into a
                                              // viewable image form (float between values 0 and 1).
}


Mat computeDFT(Mat image) {
    // http://opencv.itseez.com/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html

    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( image.rows );
    int n = getOptimalDFTSize( image.cols ); // on the border add zero values
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complex;
    merge(planes, 2, complex);         // Add to the expanded another plane with zeros

    dft(complex, complex, DFT_COMPLEX_OUTPUT);  // furier transform
    return complex;
}

void createBoxMask(Mat& box, const int& half_size_x, const int& half_size_y)
{

    /*
    int start_y = box.rows/2.0 - half_size_y;
    if(start_y<0) start_y = 0;
    int start_x = box.cols/2.0 - half_size_x;
    if(start_x<0) start_x = 0;

    for(int y=start_y; y < start_y +2*half_size_y && y < box.rows; y++) {
        for(int x=start_x; x < start_x +2*half_size_x && x <box.cols; x++) {
        box.at<int>(x,y) = 255;
       }
    }
    */
    IplImage img = box;
    // get the image data
    int height    = (&img)->height;
    int width     = (&img)->width;
    int step      = (&img)->widthStep;
    int channels  = (&img)->nChannels;
    uchar *data      = (uchar *)(&img)->imageData;
    int start_y = height/2 -  half_size_y;
    if(start_y<0) start_y = 0;
    int start_x = width/2.0 - half_size_x;
    if(start_x<0) start_x = 0;

     qDebug() << "createBoxMask channels:  " << channels << "  steps:  " << step;

    for(int i=start_y;i <start_y +2*half_size_y && i<height;i++) {
        for(int j=start_x;j<start_x +2*half_size_x && j<width;j++) {
            for(int k=1;k<channels;k++) { //loop to read for each channel
                //qDebug() << "set channel  " << i << "  " << j << " channels:  " << channels << "  " << step;
                data[i*step+j*channels+k]=1;    //inverting the image
            }
        }
    }
}

Mat createGausFilterMask(Size mask_size, int x, int y, int ksize, bool normalization, bool invert) {
    // Some corrections if out of bounds
    if(x < (ksize / 2)) {
        ksize = x * 2;
    }
    if(y < (ksize / 2)) {
        ksize = y * 2;
    }
    if(mask_size.width - x < ksize / 2 ) {
        ksize = (mask_size.width - x ) * 2;
    }
    if(mask_size.height - y < ksize / 2 ) {
        ksize = (mask_size.height - y) * 2;
    }

    // call openCV gaussian kernel generator
    double sigma = -1;
    Mat kernelX = getGaussianKernel(ksize, sigma, CV_32F);
    Mat kernelY = getGaussianKernel(ksize, sigma, CV_32F);
    // create 2d gaus
    Mat kernel = kernelX * kernelY.t();
    // create empty mask
    Mat mask = Mat::zeros(mask_size, CV_32F);
    Mat maski = Mat::zeros(mask_size, CV_32F);

    // copy kernel to mask on x,y
    Mat pos(mask, Rect(x - ksize / 2, y - ksize / 2, ksize, ksize));
    kernel.copyTo(pos);

    // create mirrored mask
    Mat posi(maski, Rect(( mask_size.width - x) - ksize / 2, (mask_size.height - y) - ksize / 2, ksize, ksize));
    kernel.copyTo(posi);
    // add mirrored to mask
    add(mask, maski, mask);

    // transform mask to range 0..1
    if(normalization) {
        normalize(mask, mask, 0, 1, NORM_MINMAX);
    }

    // invert mask
    if(invert) {
        mask = Mat::ones(mask.size(), CV_32F) - mask;
    }

    return mask;
}

