#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;

class MyHarris {
private:
  Mat img_src;
  Mat img_gray;
  int size_sobel;
  Mat img_sobel_x;
  Mat img_sobel_y;
  Mat img_sobel_x_test;
  Mat img_sobel_x2;
  Mat img_sobel_y2;
  Mat img_sobel_xy;
  Mat img_sobel_x2_g;
  Mat img_sobel_y2_g;
  Mat img_sobel_xy_g;
  Mat img_x2y2;
  Mat img_xy;
  Mat img_trace;
  Mat img_dst;
  Mat img_dst_normed;
  Mat img_dst_normed_scaled;
public:
  MyHarris(String file_path, int gauss_kernel, int thresh);
  Mat doSobelX();
  Mat doSobelY();
  void calMatrixM();
  void doGaussianBlur(int gauss_kernel);
  void calResponse();
  void drawCorners(int thresh);
};