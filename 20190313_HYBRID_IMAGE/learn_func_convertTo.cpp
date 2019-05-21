#include "learn_func_convertTo.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int myLinearChange() {
  // 读入图片
  Mat img_read = imread("D:\\NiseEngFolder\\MyDocument\\MyPictures\\lena.jpg");
  if (img_read.empty()) {
    cout << "Can not open image!" << endl;
    cin.get();
    return -1;
  }

  // 选择调节类型
  int i_input;
  cout << "请选择调节类型：" << endl;
  cout << "【1】对比度(alpha)" << endl;
  cout << "【2】亮度(beta)" << endl;
  cout << "输入对应数字：";
  cin >> i_input;

  // 初始化变量
  float f_alpha = 1.0;
  float f_beta = 50;
  float *f_change;
  if (i_input == 1) {
    f_change = &f_alpha;
  } else if (i_input == 2) {
    f_change = &f_beta;
  } else {
    cout << "请输入有效数字！" << endl;
    cin.get();
    return -1;
  }
  Mat img_change;

  while (true) {
    char key_value = waitKey(10);

    // 改变图片像素值
    img_read.convertTo(img_change, -1, f_alpha, f_beta);  // 调节对比度

    // 显示图片
    String win_name = "Changed Image";
    namedWindow(win_name, WINDOW_NORMAL);
    imshow(win_name, img_change);

    // 按键响应
    if (key_value == 'k') {
      *(f_change) *= 0.5;
    }
    if (key_value == 'j') {
      *(f_change) /= 0.5;
    }
    if (key_value == 'q') {
      break;
    }
  }
  destroyAllWindows();
  return 0;
}
