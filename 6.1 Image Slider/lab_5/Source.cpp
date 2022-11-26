#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <opencv2/core/affine.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using std::cout;
const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;
Mat src1;
Mat src2;
Mat dst;
static void on_trackbar(int, void*)
{
	alpha = (double)alpha_slider / alpha_slider_max;
	beta = (1.0 - alpha);
	//x>0 - сдвиг вправо, y>0 - сдвиг вниз => (x,y) - начальная точка
	if (alpha_slider == 0) {
		dst = src2.clone();
	}
	else if (alpha_slider == 100) {
		dst = src1.clone();
	}
	else {
		auto p1 = src2(Rect(0, 0, src2.cols * beta, src2.rows));
		auto p2 = src1(Rect(src1.cols * beta, 0, src1.cols * alpha, src1.rows));
		hconcat(p2, p1, dst);
	}
	imshow("Linear Blend", dst);
}
int main(void)
{
	src1 = imread("Winter.bmp");
	src2 = imread("Sunset.bmp");
	if (src1.empty()) { cout << "Error loading src1 \n"; return -1; }
	if (src2.empty()) { cout << "Error loading src2 \n"; return -1; }
	alpha_slider = 60;
	namedWindow("Linear Blend", WINDOW_NORMAL); // Create Window
	char TrackbarName[50];
	sprintf(TrackbarName, "", alpha_slider_max);
	createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);
	on_trackbar(alpha_slider, 0);
	waitKey(0);
	return 0;
}