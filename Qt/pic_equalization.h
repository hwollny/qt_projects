#ifndef PIC_EQUALIZATION_H
#define PIC_EQUALIZATION_H

#include <QDebug>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

void test(cv::Mat image);


void inverte(cv::Mat& image);

void FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs);

#endif // PIC_EQUALIZATION_H
