#ifndef QT_TO_OPENCV_CONVERTER_H
#define QT_TO_OPENCV_CONVERTER_H

#include <QImage>
#include <cv.h>
#include<opencv2/highgui/highgui.hpp>

//QImage Mat2QImage(cv::Mat const& src);

QImage Mat2QImage(cv::Mat &mat, QImage::Format format);

//cv::Mat QImage2Mat(QImage const& src);

cv::Mat QImage2Mat(QImage const &img, int format);


#endif // QT_TO_OPENCV_CONVERTER_H
