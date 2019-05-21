#include "learn_create_image.h"
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void myCreateImage() {
  cout << "Create Image." << endl;
  Mat img_create(200, 200, CV_8UC3, Scalar(255, 0, 0));
  String win_name = "Window for Creating Image";
  namedWindow(win_name);
  imshow(win_name, img_create);
  waitKey(0);
  destroyWindow(win_name);
}
