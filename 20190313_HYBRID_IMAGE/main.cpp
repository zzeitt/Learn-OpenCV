//#include "headfile_test.h"
//#include "learn_create_image.h"
//#include "learn_play_video.h"
//#include "learn_save_image.h"
//#include "learn_save_video.h"
//#include "learn_func_convertTo.h"
//#include "hybrid_image.h"
//#include "hybrid_image_2.h"
//#include "canny_edge_detection.h"
//#include "canny_edge_detection_2.h"
#include "descriptor_and_matching.h"

int main() {
  String path_name_1 =
    "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190522_CV_harris_detection\\Chase1.jpg";
  String path_name_2 =
    "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190522_CV_harris_detection\\Chase2.jpg";

  //MyMatch my_match(path_name_1, 9, 130, path_name_2, 9, 130);
  MyMatch my_match(path_name_1, 11, 120, path_name_2, 9, 90);
  //MyMatch my_match(path_name_1, 5, 90, path_name_2, 5, 90);
  return 0;
}