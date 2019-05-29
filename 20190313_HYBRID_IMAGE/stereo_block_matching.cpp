#include "stereo_block_matching.h"

MyDepth::MyDepth(String path_left, String path_right, int block_half_size_arg,
                 int disp_min_arg, int disp_max_arg) {
  img_left = imread(path_left, IMREAD_GRAYSCALE);
  img_right = imread(path_right, IMREAD_GRAYSCALE);
  i_row = img_left.rows;
  i_col = img_left.cols;
  block_half_size = block_half_size_arg;
  disp_min = disp_min_arg;
  disp_max = disp_max_arg;
  ;
  matrix_disparity =
      Mat::zeros(i_row, i_col, CV_8UC1);  // 初始化记录差异值的矩阵
  matrix_SSD = Mat(i_row, i_col, CV_16UC1,
                   Scalar::all(65535));  // 初始化记录SSD值的矩阵
  img_disparity = Mat::zeros(i_row, i_col, CV_8UC1);  // 初始化差异图

  // （1）特征点匹配 & 计算距离
  matchPointAndCalDisparity();
  // （2）深度图可视化
  visualizeDepth();
}

void MyDepth::matchPointAndCalDisparity() {
  /* 遍历左图 */
  for (int i = 0 + block_half_size; i < i_row - block_half_size; i++) {
    for (int j = 0 + block_half_size; j < i_col - block_half_size; j++) {
      for (int disp = disp_min; disp <= disp_max; disp++) {
        int SSD_temp = 0;
        /* 计算块内SSD */
        for (int l_r = -block_half_size + i; l_r <= block_half_size + i;
             l_r++) {
          for (int l_c = -block_half_size + j; l_c <= block_half_size + j;
               l_c++) {
            int r_r = l_r;
            int r_c = l_c + disp;

            SSD_temp +=
                pow((img_left.at<uchar>(l_r, l_c) -
                     img_right.at<uchar>(r_r, min(max(0, r_c), i_col - 1))),
                    2);
          }
        }
        /* 更新记录用矩阵 */
        if (SSD_temp < matrix_SSD.at<ushort>(i, j)) {
          matrix_disparity.at<uchar>(i, j) = disp;
          matrix_SSD.at<ushort>(i, j) = SSD_temp;
        }
      }
    }
  }
}

void MyDepth::visualizeDepth() {
  for (int i = 0; i < i_row; i++) {
    for (int j = 0; j < i_col; j++) {
      img_disparity.at<uchar>(i, j) =
          63 + (int)(192.0 * (matrix_disparity.at<uchar>(i, j) - disp_min) /
                     (disp_max - disp_min));  // 颜色越白，差异越大，深度越浅
    }
  }
  imshow("disparity", img_disparity);
  waitKey(0);
  destroyAllWindows();
}
