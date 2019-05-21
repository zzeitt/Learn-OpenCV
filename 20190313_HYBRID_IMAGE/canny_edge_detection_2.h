#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MyCanny {
 private:
  Mat kernel;         // 高斯核
  Mat img_gray;       // 灰度图
  Mat img_gauss;      // 高斯滤波后图
  Mat img_sobel;      // 边缘图
  Mat spec_angle;     // 角度图谱
  Mat img_suppress;   // 非极大值抑制后
  Mat img_threshold;  // 阈值化后最终图
  Mat img_offical;    // 官方canny
 public:
  MyCanny(String file_path, int k_size, int k_sigma, int min, int max);
  Mat createKernel(int k_size, double sigma);  // 生成高斯核
  Mat useFilter(Mat img, Mat kernel);          // 进行高斯滤波
  Mat doSobelAndAngles();                      // Sobel梯度计算
  Mat nonMaxSuppress();                        // 非极大值抑制
  Mat threshold(Mat img, int min, int max);    // 双阈值处理
};