#include "harris_detector.h"

MyHarris::MyHarris(String file_path, int gauss_kernel, int thresh) {
  // 图片初始化
  img_src = imread(file_path, IMREAD_COLOR);
  img_gray = imread(file_path, IMREAD_GRAYSCALE);
  // （1）计算梯度及其导数
  img_sobel_x = Mat(doSobelX());
  img_sobel_y = Mat(doSobelY());
  // （2）矩阵M计算
  calMatrixM();
  // （3）高斯窗口函数
  doGaussianBlur(gauss_kernel);
  // （4）计算响应R
  calResponse();
  // （5）标出角点
  drawCorners(thresh);

  // （6）显示窗口
  //imshow("IMG - SRC - CIRCLED", img_src);

  //waitKey(0);
  //destroyAllWindows();
}

Mat MyHarris::doSobelX() {
  double sobelX[] = {-1.0, 0, 1.0, -2.0, 0, 2.0, -1.0, 0, 1.0};
  Mat sobel_x(3, 3, CV_64FC1, sobelX);

  size_sobel = (int)sobel_x.rows / 2;
  // 初始化梯度图
  Mat sobeled_img = Mat(img_gray.rows - 2 * size_sobel,
                        img_gray.cols - 2 * size_sobel, CV_32FC1);
  for (int i = size_sobel; i < img_gray.rows - size_sobel; i++) {
    for (int j = size_sobel; j < img_gray.cols - size_sobel; j++) {
      double sumx = 0;
      for (int x = 0; x < sobel_x.rows; x++)
        for (int y = 0; y < sobel_x.cols; y++) {
          sumx += sobel_x.at<double>(x, y) *
                  (double)(img_gray.at<uchar>(i + x - size_sobel,
                                              j + y - size_sobel));
        }
      if (sumx > 255) sumx = 255;
      sobeled_img.at<float>(i - size_sobel, j - size_sobel) = sumx;
    }
  }
  return sobeled_img;
}

Mat MyHarris::doSobelY() {
  double sobelY[] = {1.0, 2.0, 1.0, 0, 0, 0, -1.0, -2.0, -1.0};
  Mat sobel_y(3, 3, CV_64FC1, sobelY);

  size_sobel = (int)sobel_y.rows / 2;
  // 初始化梯度图
  Mat sobeled_img = Mat(img_gray.rows - 2 * size_sobel,
                        img_gray.cols - 2 * size_sobel, CV_32FC1);
  for (int i = size_sobel; i < img_gray.rows - size_sobel; i++) {
    for (int j = size_sobel; j < img_gray.cols - size_sobel; j++) {
      double sumy = 0;
      for (int x = 0; x < sobel_y.rows; x++)
        for (int y = 0; y < sobel_y.cols; y++) {
          sumy += sobel_y.at<double>(x, y) *
                  (double)(img_gray.at<uchar>(i + x - size_sobel,
                                              j + y - size_sobel));
        }
      if (sumy > 255) sumy = 255;
      sobeled_img.at<float>(i - size_sobel, j - size_sobel) = sumy;
    }
  }
  return sobeled_img;
};

void MyHarris::calMatrixM() {
  pow(img_sobel_x, 2.0, img_sobel_x2);
  pow(img_sobel_y, 2.0, img_sobel_y2);
  multiply(img_sobel_x, img_sobel_y, img_sobel_xy);
}

void MyHarris::doGaussianBlur(int gauss_kernel) {
  GaussianBlur(img_sobel_x2, img_sobel_x2_g, Size(gauss_kernel, gauss_kernel),
               2.0, 0.0, BORDER_DEFAULT);
  GaussianBlur(img_sobel_y2, img_sobel_y2_g, Size(gauss_kernel, gauss_kernel),
               0.0, 2.0, BORDER_DEFAULT);
  GaussianBlur(img_sobel_xy, img_sobel_xy_g, Size(gauss_kernel, gauss_kernel),
               2.0, 2.0, BORDER_DEFAULT);
}

void MyHarris::calResponse() {
  multiply(img_sobel_x2_g, img_sobel_y2_g, img_x2y2);
  multiply(img_sobel_xy_g, img_sobel_xy_g, img_xy);
  pow((img_sobel_x2_g + img_sobel_y2_g), 2.0, img_trace);
  img_dst = (img_x2y2 - img_xy) - 0.04 * img_trace;

  normalize(img_dst, img_dst_normed, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
  convertScaleAbs(img_dst_normed, img_dst_normed_scaled);  // 转换为uchar
}

void MyHarris::drawCorners(int thresh) {
  img_src = img_src(Rect(size_sobel, size_sobel, img_src.cols - 2 * size_sobel,
                         img_src.rows - 2 * size_sobel));  // 标记前裁剪
  img_dst_normed_scaled_feature =
      Mat(img_src.rows, img_src.cols,
          CV_8UC(25));  // 5x5通道特征图，存储基本灰度特征
  img_dst_normed_scaled_feature = Scalar::all(0);
  for (int j = 1; j < img_src.rows - 1; j++) {
    for (int i = 1; i < img_src.cols - 1; i++) {  //减1防止遍历时越界
      if ((int)img_dst_normed_scaled.at<uchar>(j, i) > thresh) {
        if ((img_dst_normed_scaled.at<uchar>(j, i) >
             img_dst_normed_scaled.at<uchar>(j - 1, i - 1)) &&
            (img_dst_normed_scaled.at<uchar>(j, i) >
             img_dst_normed_scaled.at<uchar>(j - 1, i)) &&
            (img_dst_normed_scaled.at<uchar>(j, i) >
             img_dst_normed_scaled.at<uchar>(j - 1, i + 1)) &&
            (img_dst_normed_scaled.at<uchar>(j, i) >
             img_dst_normed_scaled.at<uchar>(j, i - 1)) &&
            (img_dst_normed_scaled.at<uchar>(j, i) >
             img_dst_normed_scaled.at<uchar>(j, i + 1)) &&
            (img_dst_normed_scaled.at<uchar>(j, i) >
             img_dst_normed_scaled.at<uchar>(j + 1, i - 1)) &&
            (img_dst_normed_scaled.at<uchar>(j, i) >
             img_dst_normed_scaled.at<uchar>(j + 1, i)) &&
            (img_dst_normed_scaled.at<uchar>(j, i) >
             img_dst_normed_scaled.at<uchar>(j + 1, i + 1))) {  //非极大值抑制
          circle(img_src, Point(i, j), 5, Scalar(255), 2, 8, 0);
          for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
              int row_temp = j + r - 2;
              int col_temp = i + c - 2;
              int feature_value_temp = 0;
              if (row_temp >= 0 && row_temp < img_dst_normed_scaled.rows &&
                    col_temp >= 0 && col_temp < img_dst_normed_scaled.cols) {
                feature_value_temp =
                    img_dst_normed_scaled.at<uchar>(row_temp, col_temp);
              }
              img_dst_normed_scaled_feature.at< Vec<uchar, 25> >(j, i)[r * 5 + c] = feature_value_temp;
            }
          }
        }
      }
    }
  }
}

Mat MyHarris::getCircledImg() {
  return img_src;
}

Mat MyHarris::getDescribedMap() {
  return img_dst_normed_scaled_feature;
}