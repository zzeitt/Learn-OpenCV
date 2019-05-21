#include "learn_func_convertTo.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int myLinearChange() {
  // ����ͼƬ
  Mat img_read = imread("D:\\NiseEngFolder\\MyDocument\\MyPictures\\lena.jpg");
  if (img_read.empty()) {
    cout << "Can not open image!" << endl;
    cin.get();
    return -1;
  }

  // ѡ���������
  int i_input;
  cout << "��ѡ��������ͣ�" << endl;
  cout << "��1���Աȶ�(alpha)" << endl;
  cout << "��2������(beta)" << endl;
  cout << "�����Ӧ���֣�";
  cin >> i_input;

  // ��ʼ������
  float f_alpha = 1.0;
  float f_beta = 50;
  float *f_change;
  if (i_input == 1) {
    f_change = &f_alpha;
  } else if (i_input == 2) {
    f_change = &f_beta;
  } else {
    cout << "��������Ч���֣�" << endl;
    cin.get();
    return -1;
  }
  Mat img_change;

  while (true) {
    char key_value = waitKey(10);

    // �ı�ͼƬ����ֵ
    img_read.convertTo(img_change, -1, f_alpha, f_beta);  // ���ڶԱȶ�

    // ��ʾͼƬ
    String win_name = "Changed Image";
    namedWindow(win_name, WINDOW_NORMAL);
    imshow(win_name, img_change);

    // ������Ӧ
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
