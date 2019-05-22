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
#include "harris_detector.h"

int main() {
  String path_name =
    //"D:\\NiseEngFolder\\MyDocument\\MyPictures\\20190410_CV_canny\\circle_"
    //"tick.png";
    //"D:\\NiseEngFolder\\MyDocument\\MyPictures\\20190410_CV_canny\\ÀÏ²ÖÓý."
    //"jpg";
    //"D:\\NiseEngFolder\\MyDocument\\MyPictures\\20190410_CV_canny\\gakki_"
    //"wall.jpg";
    "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190522_CV_harris_detection\\NotreDame1.jpg";
  MyHarris(path_name, 7, 90);
  return 0;
}