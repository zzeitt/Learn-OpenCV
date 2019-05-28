#pragma once
#include "harris_detector.h"

using namespace cv;

class MyMatch {
 private:
  MyHarris harris_left;
  MyHarris harris_right;
  Mat img_left_circled;
  Mat img_right_circled;
  Mat img_combine;
  Mat img_left_feature;
  Mat img_right_feature;
  Mat img_combine_feature;
 public:
  MyMatch(String path_left, int gauss_left, int thresh_left, String path_right,
          int gauss_right, int thresh_right);
  void combineLeftAndRight();
  void matchAndDraw();
  void showResult();

};
