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

void createBoxMask(Mat& box, const int& half_size_x, const int &half_size_y);

Mat createGausFilterMask(Size mask_size, int x, int y, int ksize, bool normalization, bool invert);

#endif // PIC_EQUALIZATION_H
