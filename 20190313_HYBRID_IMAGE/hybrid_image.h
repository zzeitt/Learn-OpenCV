#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
Mat computeDFT(Mat image);
void updateMag(Mat complex);
void shift(Mat magI);
Mat createGausFilterMask(Size mask_size, int x, int y, int ksize, bool normalization, bool invert);
Mat updateResult(Mat complex);
void doFilter(Mat complex, int x, int y, int ksize, bool normalization, bool invert);

void myHybridImage();