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
      Mat::zeros(i_row, i_col, CV_8UC1);  // ��ʼ����¼����ֵ�ľ���
  matrix_SSD = Mat(i_row, i_col, CV_16UC1,
                   Scalar::all(65535));  // ��ʼ����¼SSDֵ�ľ���
  img_disparity = Mat::zeros(i_row, i_col, CV_8UC1);  // ��ʼ������ͼ

  // ��1��������ƥ�� & �������
  matchPointAndCalDisparity();
  // ��2�����ͼ���ӻ�
  visualizeDepth();
}

void MyDepth::matchPointAndCalDisparity() {
  /* ������ͼ */
  for (int i = 0 + block_half_size; i < i_row - block_half_size; i++) {
    for (int j = 0 + block_half_size; j < i_col - block_half_size; j++) {
      for (int disp = disp_min; disp <= disp_max; disp++) {
        int SSD_temp = 0;
        /* �������SSD */
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
        /* ���¼�¼�þ��� */
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
                     (disp_max - disp_min));  // ��ɫԽ�ף�����Խ�����Խǳ
    }
  }
  imshow("disparity", img_disparity);
  waitKey(0);
  destroyAllWindows();
}
