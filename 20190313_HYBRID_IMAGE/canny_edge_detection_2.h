#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MyCanny {
 private:
  Mat kernel;         // ��˹��
  Mat img_gray;       // �Ҷ�ͼ
  Mat img_gauss;      // ��˹�˲���ͼ
  Mat img_sobel;      // ��Եͼ
  Mat spec_angle;     // �Ƕ�ͼ��
  Mat img_suppress;   // �Ǽ���ֵ���ƺ�
  Mat img_threshold;  // ��ֵ��������ͼ
  Mat img_offical;    // �ٷ�canny
 public:
  MyCanny(String file_path, int k_size, int k_sigma, int min, int max);
  Mat createKernel(int k_size, double sigma);  // ���ɸ�˹��
  Mat useFilter(Mat img, Mat kernel);          // ���и�˹�˲�
  Mat doSobelAndAngles();                      // Sobel�ݶȼ���
  Mat nonMaxSuppress();                        // �Ǽ���ֵ����
  Mat threshold(Mat img, int min, int max);    // ˫��ֵ����
};