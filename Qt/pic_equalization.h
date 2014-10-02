#ifndef PIC_EQUALIZATION_H
#define PIC_EQUALIZATION_H

#include <QDebug>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace cv;

void test(cv::Mat image);


void inverte(cv::Mat& image);

void FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs);

void shift(Mat &magI);

void updateMag(Mat complex, Mat& magI);

Mat computeDFT(Mat image);

Mat createBoxMask(Size mask_size, const int& x, const int& y, const int& ksize_x, const int& ksize_y, bool invert);

Mat createGausFilterMask(Size mask_size, const int& x, const int& y, int ksize_x, int ksize_y, bool normalization, bool invert);

void test();

#endif // PIC_EQUALIZATION_H
