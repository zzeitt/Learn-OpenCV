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
//#include "stereo_block_matching.h"

int main() {
  /* ¡¾Í¼ÏñÈÚºÏ¡¿ */
  //test();

  /* ¡¾±ßÔµ¼ì²â¡¿ */
  //String path_name = 
  //  "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190410_CV_canny\\owl.jpg";
  //MyCanny(path_name, 3, 11, 1, 250);

  /* ¡¾ÌØÕ÷Æ¥Åä¡¿ */
   String path_name_1 =
    "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190522_CV_harris_detection\\Chase1.jpg";
   String path_name_2 =
    "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190522_CV_harris_detection\\Chase2.jpg";
   MyMatch my_match(path_name_1, 11, 120, path_name_2, 9, 90);

  /* ¡¾Éî¶È¹À¼Æ¡¿ */
  //String path_left =
  //  "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190528_CV_stereo_matching\\pair0-L.png";
  //String path_right =
  //  "D:\\NiseEngFolder\\MyDocument\\MyPictures\\forMiscellaneous\\20190528_CV_stereo_matching\\pair0-R.png";
  //MyDepth(path_left, path_right, 5, -30, 30);

  return 0;
}