#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;

Mat doFourierTransform(Mat& image);
void shiftQuadrants(Mat mag);
Mat getMagnitude(Mat complex);
Mat makeGaussFilter(Size mask_size, int kernel_size, double sigma,
                    bool b_invert);
Mat doFreqFiltering(Mat complex, Mat mask);
Mat doInverseFourierTransform(Mat complex);
void getFilteredImage(Mat ptr_ret[], Mat image, int kernel, double sigma, bool invert);
void hybridImage();
void hybridImage(Mat ptr_ret[][2], Mat img_lp_src, Mat img_hp_src, int kernel_lp, double sigma_lp,
                int kernel_hp, double sigma_hp, bool invert);
void test();