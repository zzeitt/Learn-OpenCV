#include "descriptor_and_matching.h"

MyMatch::MyMatch(String path_left, int gauss_left, int thresh_left,
                 String path_right, int gauss_right, int thresh_right)
    : harris_left(path_left, gauss_left, thresh_left),
      harris_right(path_right, gauss_right, thresh_right) {
  // （1）角点检测 & 图像矩阵初始化
  img_left_circled = harris_left.getCircledImg();
  img_right_circled = harris_right.getCircledImg();
  img_left_feature = harris_left.getDescribedMap();
  img_right_feature = harris_right.getDescribedMap();
  // （2）图像合起来显示
  combineLeftAndRight();
  // （3）进行特征匹配
  matchAndDraw();
  // （4）显示结果
  showResult();
}

void MyMatch::combineLeftAndRight() {
  Size size_combine(img_left_circled.cols + img_right_circled.cols,
                    MAX(img_left_circled.rows, img_right_circled.rows));
  // 原图的拼接
  img_combine.create(size_combine, CV_MAKETYPE(img_left_circled.depth(), 3));
  img_combine = Scalar::all(0);
  Mat img_left_out, img_right_out;
  img_left_out =
      img_combine(Rect(0, 0, img_left_circled.cols, img_left_circled.rows));
  img_right_out =
      img_combine(Rect(img_left_circled.cols, 0, img_right_circled.cols,
                       img_right_circled.rows));
  img_left_circled.copyTo(img_left_out);
  img_right_circled.copyTo(img_right_out);
  // 特征图的拼接
  img_combine_feature.create(size_combine,
                             CV_MAKETYPE(img_left_feature.depth(), 25));
  img_combine_feature = Scalar::all(0);
  Mat img_left_feature_out, img_right_feature_out;
  img_left_feature_out = img_combine_feature(
      Rect(0, 0, img_left_circled.cols, img_left_circled.rows));
  img_right_feature_out =
      img_combine_feature(Rect(img_left_circled.cols, 0, img_right_circled.cols,
                               img_right_circled.rows));
  img_left_feature.copyTo(img_left_feature_out);
  img_right_feature.copyTo(img_right_feature_out);
}

void MyMatch::matchAndDraw() {
  Mat mat_temp = Mat(1, 1, CV_8UC(25));
  mat_temp = Scalar::all(0);
  Vec<uchar, 25> vector_zero =
      mat_temp.at<Vec<uchar, 25> >(0, 0);  // 生成全零向量
  Mat img_right_mark =
      Mat(img_right_circled.rows, img_right_circled.cols, CV_8UC1);
  img_right_mark = Scalar::all(0);
  /* 遍历左图 */
  for (int r_left = 0; r_left < img_left_circled.rows; r_left++) {
    for (int c_left = 0; c_left < img_left_circled.cols; c_left++) {
      if (img_combine_feature.at<Vec<uchar, 25> >(r_left, c_left) !=
          vector_zero) {  // 跳过零向量
        int r_draw_start = r_left;
        int c_draw_start = c_left;
        int r_draw_end = 0;
        int c_draw_end = img_left_circled.cols;
        float distance_max = 0.0;
        /* 遍历右图 */
        for (int r_right = 0; r_right < img_right_feature.rows; r_right++) {
          for (int c_right = img_left_circled.cols;
               c_right < img_right_circled.cols + img_left_circled.cols;
               c_right++) {
            if (img_combine_feature.at<Vec<uchar, 25> >(r_right, c_right) !=
                vector_zero) {  // 跳过零向量
              if (img_right_mark.at<uchar>(
                      r_right, c_right - img_left_circled.cols) != 1) {
                // 跳过已配对的点
                float distance_temp = norm(
                    img_combine_feature.at<Vec<uchar, 25> >(r_left, c_left),
                    img_combine_feature.at<Vec<uchar, 25> >(r_right, c_right),
                    NORM_L2);
                if (distance_temp > distance_max) {
                  r_draw_end = r_right;
                  c_draw_end = c_right;
                  distance_max = distance_temp;
                }
              }
            }
          }
        }
        cv::line(img_combine, Point(c_draw_start, r_draw_start),
                 Point(c_draw_end, r_draw_end), Scalar(0, 0, 255), 1, 8, 0);
        img_right_mark.at<uchar>(r_draw_end,
                                 c_draw_end - img_left_circled.cols) = 1;
        // 标记为已配对
      }
    }
  }
}

void MyMatch::showResult() {
  namedWindow("Combined Image", WINDOW_NORMAL);
  imshow("Combined Image", img_combine);

  waitKey(0);
  destroyAllWindows();
}
