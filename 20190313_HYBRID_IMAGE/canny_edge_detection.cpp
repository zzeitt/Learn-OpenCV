#include "canny_edge_detection.h"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat makeGaussianKernel(int k_size, double sigma) {
  Mat kernelX = getGaussianKernel(k_size, sigma, CV_32F);
  Mat kernelY = getGaussianKernel(k_size, sigma, CV_32F);
  Mat kernel = kernelX * kernelY.t();            // ×ªÖÃ£¬Ïà³Ë
  return kernel;
}

void doCanny() {
  Mat k_gauss = makeGaussianKernel(3, 1);
  cout << "kernel=" << endl << k_gauss << endl;

  cin.get();
}
