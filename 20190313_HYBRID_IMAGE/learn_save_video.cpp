#include "learn_save_video.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int mySaveVideo() {
  //读取视频文件
  VideoCapture cap(
      "D:\\NiseEngFolder\\MyDocument\\MyVideos\\forPopping\\GROOVE LINE 2015 "
      "沖縄予選 2次オーディション KITE (フォーマーアクション).mp4");
  if (cap.isOpened() == false) {
    cout << "Cannot open the video file!" << endl;
    cin.get();
    return -1;
  }

  //创建写视频的类
  int frame_width = static_cast<int>(
      cap.get(CAP_PROP_FRAME_WIDTH));  // get the width of frames of the video
  int frame_height = static_cast<int>(
      cap.get(CAP_PROP_FRAME_HEIGHT));  // get the height of frames of the video
  Size frame_size(frame_width, frame_height);
  int frames_per_second = 10;
  // Create and initialize the VideoWriter object
  VideoWriter oVideoWriter(
      "D:\\NiseEngFolder\\MyDocument\\MyVideos\\forPopping\\tmp.mp4",
      VideoWriter::fourcc('M', 'P', '4', '2'), frames_per_second, frame_size,
      true);
  // If the VideoWriter object is not initialized successfully, exit the program
  if (oVideoWriter.isOpened() == false) {
    cout << "Cannot save the video to a file" << endl;
    cin.get();  // wait for any key press
    return -1;
  }

  string window_name = "My Camera Feed";
  namedWindow(window_name);  // create a window called "My Camera Feed"

  while (true) {
    Mat frame;
    bool isSuccess = cap.read(frame);  // read a new frame from the video camera

    // Breaking the while loop if frames cannot be read from the camera
    if (isSuccess == false) {
      cout << "Video camera is disconnected" << endl;
      cin.get();  // Wait for any key press
      break;
    }

    // write the video frame to the file
    oVideoWriter.write(frame);

    // show the frame in the created window
    imshow(window_name, frame);

    // Wait for for 10 milliseconds until any key is pressed.
    // If the 'Esc' key is pressed, break the while loop.
    // If any other key is pressed, continue the loop
    // If any key is not pressed within 10 milliseconds, continue the loop
    if (waitKey(10) == 27) {
      cout << "Esc key is pressed by the user. Stopping the video" << endl;
      break;
    }
  }

  // Flush and close the video file
  oVideoWriter.release();

  return 0;
}
