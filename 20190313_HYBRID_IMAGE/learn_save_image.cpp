#include "learn_save_image.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int mySaveImage() {
  //����ͼƬ
  Mat img_read = imread("D:\\NiseEngFolder\\MyDocument\\MyPictures\\lena.jpg");
  if (img_read.empty()) {
    cout << "Can not open image!" << endl;
    cin.get();
    return -1;
  }

  //д��ͼƬ
  Mat img_save(img_read);  //�����Կ���
  char char_confirm;
  cout << "Sure to save the image? (Y/n)" << endl;
  cin >> char_confirm;
  bool is_confirm = (char_confirm == 'Y');
  bool is_success =
      imwrite("D:\\NiseEngFolder\\MyDocument\\MyPictures\\tmp.jpg", img_save);
  if ((is_success == false) || (is_confirm == false)) {
    cout << "Failed to save the image!" << endl;
    cin.get();
    return -1;
  } else {
    cout << "Image is saved." << endl;
    //��ʾͼƬ
    String win_name = "Saved Image";
    namedWindow(win_name, WINDOW_NORMAL);
    imshow(win_name, img_save);
    waitKey(0);
    destroyWindow(win_name);
  }

  return 0;
}
