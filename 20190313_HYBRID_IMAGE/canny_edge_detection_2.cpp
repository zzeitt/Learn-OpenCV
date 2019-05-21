#include "canny_edge_detection_2.h"

MyCanny::MyCanny(String file_path, int k_size, int k_sigma, int min, int max) {
  img_gray = imread(file_path, IMREAD_GRAYSCALE);
  if (img_gray.empty()) {
    cout << "Can not open image!" << endl;
  } else {
    kernel = Mat(createKernel(k_size, k_sigma));   // 高斯核
    img_gauss = Mat(useFilter(img_gray, kernel));  // 进行高斯滤波
    img_sobel = Mat(doSobelAndAngles());           // 进行Sobel梯度处理
    img_suppress = Mat(nonMaxSuppress());          // 进行非极大值抑制
    img_threshold = Mat(threshold(img_suppress, min, max));  // 进行双阈值处理
    Canny(img_gray, img_offical, min, max);

    namedWindow("IMG - GRAY");
    namedWindow("IMG - GAUSSIAN");
    namedWindow("IMG - SOBEL");
    namedWindow("IMG - SUPPRESS");
    namedWindow("IMG - DST");
    namedWindow("IMG - OFFICIAL");

    imshow("IMG - GRAY", img_gray);
    imshow("IMG - GAUSSIAN", img_gauss);
    imshow("IMG - SOBEL", img_sobel);
    imshow("IMG - SUPPRESS", img_suppress);
    imshow("IMG - DST", img_threshold);
    imshow("IMG - OFFICIAL", img_offical);

    waitKey(0);
    destroyAllWindows();
  }
}

Mat MyCanny::createKernel(int k_size, double sigma) {
  Mat kernelX = getGaussianKernel(k_size, sigma, CV_32F);
  Mat kernelY = getGaussianKernel(k_size, sigma, CV_32F);
  Mat kernel_ret = kernelX * kernelY.t();  // 转置，相乘

  return kernel_ret;
}

Mat MyCanny::useFilter(Mat img, Mat kernel_in) {
  int size = (int)kernel_in.rows / 2;
  Mat filtered_img = Mat(img.rows - 2 * size, img.cols - 2 * size, CV_8UC1);
  for (int i = size; i < img.rows - size; i++) {
    for (int j = size; j < img.cols - size; j++) {
      double sum = 0;

      for (int x = 0; x < kernel_in.cols; x++)
        for (int y = 0; y < kernel_in.rows; y++) {
          sum += kernel_in.at<float>(x, y) *
                 (double)(img.at<uchar>(i + x - size, j + y - size));
        }

      filtered_img.at<uchar>(i - size, j - size) = sum;
    }
  }
  return filtered_img;
}

Mat MyCanny::doSobelAndAngles() {
  double sobelX[] = {-1.0, 0, 1.0, -2.0, 0, 2.0, -1.0, 0, 1.0};
  double sobelY[] = {1.0, 2.0, 1.0, 0, 0, 0, -1.0, -2.0, -1.0};
  Mat sobel_x(3, 3, CV_64FC1, sobelX);
  Mat sobel_y(3, 3, CV_64FC1, sobelY);

  int size = (int)sobel_x.rows / 2;
  // 初始化图与谱
  Mat filtered_img =
      Mat(img_gauss.rows - 2 * size, img_gauss.cols - 2 * size, CV_8UC1);
  spec_angle =
      Mat(img_gauss.rows - 2 * size, img_gauss.cols - 2 * size, CV_32FC1);
  for (int i = size; i < img_gauss.rows - size; i++) {
    for (int j = size; j < img_gauss.cols - size; j++) {
      double sumx = 0;
      double sumy = 0;

      for (int x = 0; x < sobel_x.rows; x++)
        for (int y = 0; y < sobel_x.cols; y++) {
          sumx += sobel_x.at<double>(x, y) *
                  (double)(img_gauss.at<uchar>(
                      i + x - size, j + y - size));  // Sobel_X Filter Value
          sumy += sobel_y.at<double>(x, y) *
                  (double)(img_gauss.at<uchar>(
                      i + x - size, j + y - size));  // Sobel_Y Filter Value
        }
      double sumxsq = sumx * sumx;
      double sumysq = sumy * sumy;

      double sq2 = sqrt(sumxsq + sumysq);

      if (sq2 > 255)  // Unsigned Char Fix
        sq2 = 255;
      filtered_img.at<uchar>(i - size, j - size) = sq2;

      if (sumx == 0)  // Arctan Fix
        spec_angle.at<float>(i - size, j - size) = 90;
      else
        spec_angle.at<float>(i - size, j - size) = atan(sumy / sumx);
    }
  }

  return filtered_img;
}

Mat MyCanny::nonMaxSuppress() {
  Mat filtered_img = Mat(img_sobel.rows - 2, img_sobel.cols - 2, CV_8UC1);
  for (int i = 1; i < img_sobel.rows - 1; i++) {
    for (int j = 1; j < img_sobel.cols - 1; j++) {
      float angle_tan = spec_angle.at<float>(i, j);

      filtered_img.at<uchar>(i - 1, j - 1) = img_sobel.at<uchar>(i, j);
      // 水平方向
      if (((-22.5 < angle_tan) && (angle_tan <= 22.5)) ||
          ((157.5 < angle_tan) && (angle_tan <= -157.5))) {
        if ((img_sobel.at<uchar>(i, j) < img_sobel.at<uchar>(i, j + 1)) ||
            (img_sobel.at<uchar>(i, j) < img_sobel.at<uchar>(i, j - 1)))
          filtered_img.at<uchar>(i - 1, j - 1) = 0;
      }
      // 竖直方向
      if (((-112.5 < angle_tan) && (angle_tan <= -67.5)) ||
          ((67.5 < angle_tan) && (angle_tan <= 112.5))) {
        if ((img_sobel.at<uchar>(i, j) < img_sobel.at<uchar>(i + 1, j)) ||
            (img_sobel.at<uchar>(i, j) < img_sobel.at<uchar>(i - 1, j)))
          filtered_img.at<uchar>(i - 1, j - 1) = 0;
      }

      //-45 度方向
      if (((-67.5 < angle_tan) && (angle_tan <= -22.5)) ||
          ((112.5 < angle_tan) && (angle_tan <= 157.5))) {
        if ((img_sobel.at<uchar>(i, j) < img_sobel.at<uchar>(i - 1, j + 1)) ||
            (img_sobel.at<uchar>(i, j) < img_sobel.at<uchar>(i + 1, j - 1)))
          filtered_img.at<uchar>(i - 1, j - 1) = 0;
      }

      // 45 度方向
      if (((-157.5 < angle_tan) && (angle_tan <= -112.5)) ||
          ((22.5 < angle_tan) && (angle_tan <= 67.5))) {
        if ((img_sobel.at<uchar>(i, j) < img_sobel.at<uchar>(i + 1, j + 1)) ||
            (img_sobel.at<uchar>(i, j) < img_sobel.at<uchar>(i - 1, j - 1)))
          filtered_img.at<uchar>(i - 1, j - 1) = 0;
      }
    }
  }
  return filtered_img;
}

Mat MyCanny::threshold(Mat img, int min, int max) {
  if (min > 255) min = 255;
  if (max > 255) max = 255;

  Mat filtered_img = Mat(img.rows, img.cols, img.type());

  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      filtered_img.at<uchar>(i, j) = img.at<uchar>(i, j);
      if (filtered_img.at<uchar>(i, j) > max)
        filtered_img.at<uchar>(i, j) = 255;
      else if (filtered_img.at<uchar>(i, j) < min)
        filtered_img.at<uchar>(i, j) = 0;
      else {
        // 如果亮度值介于最大最小之间，进行8邻域判定
        bool b_high = false;
        for (int x = i - 1; x <= i + 1; x++) {
          for (int y = j - 1; y <= j + 1; y++) {
            if (x < 0 || y < 0 || x >= filtered_img.rows ||
                y >= filtered_img.cols)  // 越界
              continue;
            else {
              if (filtered_img.at<uchar>(x, y) > max) {
                filtered_img.at<uchar>(i, j) = 255;
                b_high = true;
                break;
              }
            }
          }
          if (b_high) break;  // 一旦检测到强值，跳出
        }
        // 未搜索到，判为弱值
        if (!b_high) filtered_img.at<uchar>(i, j) = 0;
      }
    }
  }
  return filtered_img;
}
