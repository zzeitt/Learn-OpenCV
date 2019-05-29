#include "hybrid_image_2.h"
#include <iostream>

using namespace cv;
using namespace std;

Mat doFourierTransform(Mat& image) {
  // �˺���������ͼƬ�ĸ���Ҷ�任���
  // �Զ�ͨ���������ʽ����
  Mat padded;
  int m = getOptimalDFTSize(image.rows);
  int n = getOptimalDFTSize(image.cols);
  // create output image of optimal size
  copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));
  image = padded;  // ��Ȼ�ߴ���ˣ���ԭͼҲҪ���ű�
  // copy the source image, on the border add zero values
  Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
  // create a complex matrix
  Mat complex;
  merge(planes, 2, complex);
  dft(complex, complex, DFT_COMPLEX_OUTPUT);  // fourier transform
  return complex;  // complex ��һ�������������ֱ��Ӧʵ���鲿
}

void shiftQuadrants(Mat mag) {
  // ��ԭͼ���вü�������-2���Ĳ��룩��λ�룬
  // �൱���롰�����һλΪ0����ȫһ�����룬
  // ��֤������������ƽ��
  mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));

  int cx = mag.cols / 2;
  int cy = mag.rows / 2;

  Mat q0(mag, Rect(0, 0, cx, cy));    // ����
  Mat q1(mag, Rect(cx, 0, cx, cy));   // ����
  Mat q2(mag, Rect(0, cy, cx, cy));   // ����
  Mat q3(mag, Rect(cx, cy, cx, cy));  // ����

  Mat tmp;
  // �������¶Ե�
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);
  // �������¶Ե�
  q1.copyTo(tmp);
  q2.copyTo(q1);
  tmp.copyTo(q2);
}

Mat getMagnitude(Mat complex) {
  Mat mag;
  Mat planes[] = {Mat::zeros(complex.size(), CV_32F),
                  Mat::zeros(complex.size(), CV_32F)};
  split(complex, planes);
  magnitude(planes[0], planes[1], mag);  // ������
  mag += Scalar::all(1);                 // ȫԱ��һ
  log(mag, mag);                         // ȡ����
  shiftQuadrants(mag);
  normalize(mag, mag, 1, 0, NORM_INF);
  return mag;
}

Mat makeGaussFilter(Size mask_size, int kernel_size, double sigma,
                    bool b_invert) {
  // ��ֹ��Խ��
  int size_tmp = ((mask_size.width < mask_size.height) ? mask_size.width
                                                       : mask_size.height);
  if (kernel_size > size_tmp) {
    kernel_size = size_tmp;
  }
  if (kernel_size < 1) {
    kernel_size = 1;
  }
  // ���ɸ�˹��
  // double sigma = -1;
  Mat kernelX = getGaussianKernel(kernel_size, sigma, CV_32F);
  Mat kernelY = getGaussianKernel(kernel_size, sigma, CV_32F);
  Mat kernel = kernelX * kernelY.t();  // ת�ã����
  Mat mask = Mat::zeros(mask_size, CV_32F);
  Mat pos(mask, Rect(mask_size.width / 2 - kernel_size / 2,
                     mask_size.height / 2 - kernel_size / 2, kernel_size,
                     kernel_size));
  kernel.copyTo(pos);
  normalize(mask, mask, 0, 1, NORM_MINMAX);  // ��һ��
  //�Ƿ�ڰ׷�ɫ
  if (b_invert) {
    mask = Mat::ones(mask_size, CV_32F) - mask;
  }
  return mask;
}

Mat doFreqFiltering(Mat complex, Mat mask) {
  shiftQuadrants(mask);  // �洢��complex�е���δ���Ļ���Ƶ��
  Mat planes[] = {Mat::zeros(complex.size(), CV_32F),
                  Mat::zeros(complex.size(), CV_32F)};
  planes[0] = mask;
  planes[1] = mask;
  Mat mask_2channels;
  merge(planes, 2, mask_2channels);
  // ��Ĥ������ͼƬ�ĸ����������
  mulSpectrums(complex, mask_2channels, complex, DFT_ROWS);
  return complex;
}

Mat doInverseFourierTransform(Mat complex) {
  Mat image_after;
  idft(complex, image_after);
  Mat planes[] = {Mat::zeros(complex.size(), CV_32F),
                  Mat::zeros(complex.size(), CV_32F)};
  split(image_after, planes);
  magnitude(planes[0], planes[1], image_after);
  normalize(image_after, image_after, 0, 1, NORM_MINMAX);
  return image_after;
}

void getFilteredImage(Mat ptr_ret[], Mat image, int kernel, double sigma,
                      bool invert) {
  Mat img_complex = doFourierTransform(image);
  Mat img_mask = makeGaussFilter(image.size(), kernel, sigma, invert);
  img_complex = doFreqFiltering(img_complex, img_mask);
  Mat img_after = doInverseFourierTransform(img_complex);
  Mat img_after_mag = getMagnitude(img_complex);
  ptr_ret[0] = img_after;
  ptr_ret[1] = img_after_mag;
}

void hybridImage() {
  Mat img_lp_src = imread(
    "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190403_CV_hybrid_image\\girl_cr.jpg",
      //"D:\\NiseEngFolder\\MyDocument\\MyPictures\\20190403_CV_hybrid_"
      //"image\\lena_gray.jpg",
      IMREAD_UNCHANGED);
  Mat img_hp_src = imread(
    "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190403_CV_hybrid_image\\lena_cr.jpg",
      //"D:\\NiseEngFolder\\MyDocument\\MyPictures\\20190403_CV_hybrid_"
      //"image\\owl.jpg",
      IMREAD_UNCHANGED);
  // �ߴ�ͳһ
  int width =
      img_lp_src.cols < img_hp_src.cols ? img_lp_src.cols : img_hp_src.cols;
  int height =
      img_lp_src.rows < img_hp_src.rows ? img_lp_src.rows : img_hp_src.rows;
  img_lp_src = img_lp_src(Rect(0, 0, width, height));
  img_hp_src = img_hp_src(Rect(0, 0, width, height));
  // ͨ������
  Mat img_lp_planes[] = {Mat::zeros(img_lp_src.size(), CV_32F),
                         Mat::zeros(img_lp_src.size(), CV_32F),
                         Mat::zeros(img_lp_src.size(), CV_32F)};
  split(img_lp_src, img_lp_planes);
  Mat img_hp_planes[] = {Mat::zeros(img_hp_src.size(), CV_32F),
                         Mat::zeros(img_hp_src.size(), CV_32F),
                         Mat::zeros(img_hp_src.size(), CV_32F)};
  split(img_hp_src, img_hp_planes);
  Mat array_lp_r[2];
  Mat array_lp_g[2];
  Mat array_lp_b[2];
  Mat array_hp_r[2];
  Mat array_hp_g[2];
  Mat array_hp_b[2];
  getFilteredImage(array_lp_r, img_lp_planes[0], 80, 35, false);
  getFilteredImage(array_lp_g, img_lp_planes[1], 80, 35, false);
  getFilteredImage(array_lp_b, img_lp_planes[2], 80, 35, false);
  getFilteredImage(array_hp_r, img_hp_planes[0], 40, 295, true);
  getFilteredImage(array_hp_g, img_hp_planes[1], 40, 295, true);
  getFilteredImage(array_hp_b, img_hp_planes[2], 40, 295, true);
  img_lp_planes[0] = array_lp_r[0];
  img_lp_planes[1] = array_lp_g[0];
  img_lp_planes[2] = array_lp_b[0];
  img_hp_planes[0] = array_hp_r[0];
  img_hp_planes[1] = array_hp_g[0];
  img_hp_planes[2] = array_hp_b[0];
  imshow("tmp", img_lp_planes[1]);
  // merge(img_lp_planes, 3, img_lp_src);
  // merge(img_hp_planes, 3, img_hp_src);
  // Mat img_hybrid = img_lp_src + img_hp_src;
  // imshow("Hybrid Image", img_hybrid);
  waitKey(0);
  destroyAllWindows();
}

void hybridImage(Mat ptr_ret[][2], Mat img_lp_src, Mat img_hp_src,
                 int kernel_lp, double sigma_lp, int kernel_hp, double sigma_hp,
                 bool invert) {
  // �ü�ͼƬ
  int width =
      img_lp_src.cols < img_hp_src.cols ? img_lp_src.cols : img_hp_src.cols;
  int height =
      img_lp_src.rows < img_hp_src.rows ? img_lp_src.rows : img_hp_src.rows;
  img_lp_src = img_lp_src(Rect(0, 0, width, height));
  img_hp_src = img_hp_src(Rect(0, 0, width, height));
  // �ں�ͼƬ
  Mat array_lp[2];
  Mat array_hp[2];
  Mat array_hybrid[2];
  getFilteredImage(array_lp, img_lp_src, kernel_lp, sigma_lp, invert);
  getFilteredImage(array_hp, img_hp_src, kernel_hp, sigma_hp, !(invert));
  Mat img_hybrid = array_lp[0] + array_hp[0];
  array_hybrid[0] = img_hybrid;
  ptr_ret[0][0] = array_lp[0];
  ptr_ret[0][1] = array_lp[1];
  ptr_ret[1][0] = array_hp[0];
  ptr_ret[1][1] = array_hp[1];
  ptr_ret[2][0] = array_hybrid[0];
}
void test() {
  Mat img_lp_src = imread(
    "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190403_CV_hybrid_image\\girl_cr.jpg",
      //"D:\\NiseEngFolder\\MyDocument\\MyPictures\\20190403_CV_hybrid_"
      //"image\\lena_gray.jpg",
      // IMREAD_UNCHANGED);
      IMREAD_GRAYSCALE);
  Mat img_hp_src = imread(
    "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190403_CV_hybrid_image\\lena_cr.jpg",
      //"D:\\NiseEngFolder\\MyDocument\\MyPictures\\20190403_CV_hybrid_"
      //"image\\owl.jpg",
      // IMREAD_UNCHANGED);
      IMREAD_GRAYSCALE);
  Mat img_all[3][2], img_hybrid, img_lp_mag, img_hp_mag;
  int kernel_lp = 80, kernel_hp = 40;
  double sigma_lp = 35, sigma_hp = 295;
  bool invert = false;
  char key_value;
  String win_name_hybrid = "Hybrid Image", win_name_lp_mag = "LP Magnitude",
         win_name_hp_mag = "HP Magnitude";
  namedWindow(win_name_hybrid, WINDOW_NORMAL);
  namedWindow(win_name_lp_mag, WINDOW_NORMAL);
  namedWindow(win_name_hp_mag, WINDOW_NORMAL);
  while (true) {
    // ͼƬ������
    hybridImage(img_all, img_lp_src, img_hp_src, kernel_lp, sigma_lp, kernel_hp,
                sigma_hp, invert);
    img_lp_mag = img_all[0][1];
    img_hp_mag = img_all[1][1];
    img_hybrid = img_all[2][0];
    // ��ʾͼƬ
    imshow(win_name_hybrid, img_hybrid);
    imshow(win_name_lp_mag, img_lp_mag);
    imshow(win_name_hp_mag, img_hp_mag);
    // ������Ӧ
    {
      key_value = waitKey(10);
      if (key_value == 'd') {
        kernel_lp += 10;
        if (kernel_lp > 2000) {  // ��ֹ����Խ��
          kernel_lp = 2000;
        }
      }
      if (key_value == 's') {
        kernel_lp -= 10;
        if (kernel_lp <= 0) {  // ��ֹ����Խ��
          kernel_lp = 1;
        }
      }
      if (key_value == 'f') {
        sigma_lp += 5;
        if (sigma_lp >= 1000) {  // ��ֹ����Խ��
          sigma_lp = 1000;
        }
      }
      if (key_value == 'a') {
        sigma_lp -= 5;
        if (sigma_lp <= 1) {  // ��ֹ����Խ��
          sigma_lp = 1;
        }
      }
      if (key_value == 'k') {
        kernel_hp += 10;
        if (kernel_hp > 2000) {  // ��ֹ����Խ��
          kernel_hp = 2000;
        }
      }
      if (key_value == 'j') {
        kernel_hp -= 10;
        if (kernel_hp <= 0) {  // ��ֹ����Խ��
          kernel_hp = 1;
        }
      }
      if (key_value == 'l') {
        sigma_hp += 5;
        if (sigma_hp >= 1000) {  // ��ֹ����Խ��
          sigma_hp = 1000;
        }
      }
      if (key_value == 'h') {
        sigma_hp -= 5;
        if (sigma_hp <= 1) {  // ��ֹ����Խ��
          sigma_hp = 1;
        }
      }
      if (key_value == 'i') {
        invert = !(invert);
      }
      if (key_value == 'q') {
        break;
      }
      if (key_value != -1) {
        cout << "key_value: " << (char)(key_value + 'A' - 'a') << endl;
        cout << "kernel_lp: " << kernel_lp << endl;
        cout << "sigma_lp: " << sigma_lp << endl;
        cout << "kernel_hp: " << kernel_hp << endl;
        cout << "sigma_hp: " << sigma_hp << endl;
      }
    }
  }
  destroyAllWindows();
}