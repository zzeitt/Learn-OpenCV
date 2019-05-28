#pragma once
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class MyDepth {
 private:
  Mat img_left;
  Mat img_right;
  int i_row, i_col, block_half_size, disp_min, disp_max;
  Mat matrix_SSD;
  Mat matrix_disparity;
  Mat img_disparity;

 public:
  MyDepth(String path_left, String path_right, int block_half_size_arg,
          int disp_min_arg, int disp_max_arg);
  void matchPointAndCalDisparity();
  void visualizeDepth();
};
