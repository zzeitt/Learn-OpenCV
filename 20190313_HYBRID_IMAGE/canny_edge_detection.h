#pragma once
#include<opencv2/opencv.hpp>

using namespace cv;

Mat makeGaussianKernel(int k_size, double sigma);
void doCanny();