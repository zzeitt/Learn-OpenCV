#include "learn_play_video.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int myPlayVideo() {
  VideoCapture cap(
      "D:\\NiseEngFolder\\MyDocument\\MyVideos\\forPopping\\GROOVE LINE 2015 "
      "沖縄予選 2次オーディション KITE (フォーマーアクション).mp4");
  if (cap.isOpened() == false) {
    cout << "Cannot open the video file." << endl;
    cin.get();
    return -1;
  }

  double fps = cap.get(CAP_PROP_FPS);
  cout << "Frames per seconds : " << fps << endl;
  String win_name = "Kite Dance";
  namedWindow(win_name, WINDOW_NORMAL);

  //播放视频
  while (true) {
    Mat frame;
    bool b_success = cap.read(frame);
    if (b_success == false) {
      cout << "Video finished." << endl;
      break;
    }
    imshow(win_name, frame);

    //设置ESC键
    if (waitKey(10) == 27) {
      cout << "ESC key is pressed. Stop playing." << endl;
      break;
    }
  }
  return 0;
}
