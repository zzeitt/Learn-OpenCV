#include "hybrid_image.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat computeDFT(Mat image) {
  Mat padded;
  int m = getOptimalDFTSize(image.rows);
  int n = getOptimalDFTSize(image.cols);
  // create output image of optimal size
  copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));
  // copy the source image, on the border add zero values
  Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
  // create a complex matrix
  Mat complex;
  merge(planes, 2, complex);
  dft(complex, complex, DFT_COMPLEX_OUTPUT);  // fourier transform
  return complex;
}

void updateMag(Mat complex) {
  Mat magI;
  Mat planes[] = {Mat::zeros(complex.size(), CV_32F),
                  Mat::zeros(complex.size(), CV_32F)};
  split(complex, planes);  // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
  magnitude(planes[0], planes[1], magI);  // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
  // switch to logarithmic scale: log(1 + magnitude)
  magI += Scalar::all(1);
  log(magI, magI);
  shift(magI);  // rearrage quadrants
  // Transform the magnitude matrix into a viewable image (float values 0-1)
  normalize(magI, magI, 1, 0, NORM_INF);
  imshow("spectrum", magI);
}

void shift(Mat magI) {
  // crop if it has an odd number of rows or columns
  magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

  int cx = magI.cols / 2;
  int cy = magI.rows / 2;

  Mat q0(magI, Rect(0, 0, cx, cy));    // Top-Left - Create a ROI per quadrant
  Mat q1(magI, Rect(cx, 0, cx, cy));   // Top-Right
  Mat q2(magI, Rect(0, cy, cx, cy));   // Bottom-Left
  Mat q3(magI, Rect(cx, cy, cx, cy));  // Bottom-Right

  Mat tmp;  // swap quadrants (Top-Left with Bottom-Right)
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);
  q1.copyTo(tmp);  // swap quadrant (Top-Right with Bottom-Left)
  q2.copyTo(q1);
  tmp.copyTo(q2);
}

Mat createGausFilterMask(Size mask_size, int x, int y, int ksize,
                         bool normalization, bool invert) {
  // Some corrections if out of bounds
  if (x < (ksize / 2)) {
    ksize = x * 2;
  }
  if (y < (ksize / 2)) {
    ksize = y * 2;
  }
  if (mask_size.width - x < ksize / 2) {
    ksize = (mask_size.width - x) * 2;
  }
  if (mask_size.height - y < ksize / 2) {
    ksize = (mask_size.height - y) * 2;
  }

  // call openCV gaussian kernel generator
  double sigma = -1;
  Mat kernelX = getGaussianKernel(ksize, sigma, CV_32F);
  Mat kernelY = getGaussianKernel(ksize, sigma, CV_32F);
  // create 2d gaus
  Mat kernel = kernelX * kernelY.t();
  // create empty mask
  Mat mask = Mat::zeros(mask_size, CV_32F);
  Mat maski = Mat::zeros(mask_size, CV_32F);

  // copy kernel to mask on x,y
  Mat pos(mask, Rect(x - ksize / 2, y - ksize / 2, ksize, ksize));
  kernel.copyTo(pos);

  // create mirrored mask
  Mat posi(maski, Rect((mask_size.width - x) - ksize / 2,
                       (mask_size.height - y) - ksize / 2, ksize, ksize));
  kernel.copyTo(posi);
  // add mirrored to mask
  add(mask, maski, mask);

  // transform mask to range 0..1
  if (normalization) {
    normalize(mask, mask, 0, 1, NORM_MINMAX);
  }

  // invert mask
  if (invert) {
    mask = Mat::ones(mask.size(), CV_32F) - mask;
  }

  return mask;
}

Mat updateResult(Mat complex) {
  Mat work;
  idft(complex, work);
  //  dft(complex, work, DFT_INVERSE + DFT_SCALE);
  Mat planes[] = {Mat::zeros(complex.size(), CV_32F),
                  Mat::zeros(complex.size(), CV_32F)};
  split(work, planes);  // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

  magnitude(planes[0], planes[1],
            work);  // === sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
  normalize(work, work, 0, 1, NORM_MINMAX);
  // imshow("result", work);
  return work;
}

void doFilter(Mat complex, int ksize, bool normalization, bool invert) {
  int x = complex.cols / 2;
  int y = complex.rows / 2;
  Mat mask =
      createGausFilterMask(complex.size(), x, y, ksize, normalization, invert);
  // show the kernel
  imshow("gaus-mask", mask);

  shift(mask);

  Mat planes[] = {Mat::zeros(complex.size(), CV_32F),
                  Mat::zeros(complex.size(), CV_32F)};
  Mat kernel_spec;
  planes[0] = mask;  // real
  planes[1] = mask;  // imaginar
  merge(planes, 2, kernel_spec);

  mulSpectrums(complex, kernel_spec, complex,
               DFT_ROWS);  // only DFT_ROWS accepted
}

void myHybridImage() {
  // 读入图片
  Mat img_lena = imread("D:\\NiseEngFolder\\MyDocument\\MyPictures\\lena.jpg",
                        IMREAD_GRAYSCALE);
  Mat img_owl = imread("D:\\NiseEngFolder\\MyDocument\\MyPictures\\owl.jpg",
                       IMREAD_GRAYSCALE);
  // 处理图片
  Mat img_lena_complex = computeDFT(img_lena);
  doFilter(img_lena_complex, 5000, true, false);  // 最后一个参数表是否高通
  //updateMag(img_lena_complex);                    // show spectrum
  Mat img_lena_LPF = updateResult(img_lena_complex);  // do inverse transform

  Mat img_owl_complex = computeDFT(img_owl);
  doFilter(img_owl_complex, 10, true, true);  // 最后一个参数表是否高通
  // updateMag(img_owl_complex);     // show spectrum
  Mat img_owl_HPF = updateResult(img_owl_complex);  // do inverse transform

  imshow("lena", img_lena_LPF);
  imshow("owl", img_owl_HPF);

  // 销毁窗口
  waitKey(0);
  destroyAllWindows();
}
