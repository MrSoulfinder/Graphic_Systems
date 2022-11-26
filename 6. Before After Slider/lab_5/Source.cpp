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
	
	Mat p1, p2;
	if (alpha_slider == 0) {
		dst = src1.clone();
	}
	else if (alpha_slider == 100) {
		dst = src2.clone();
	}
	else {
		p1 = src2(Rect(0, 0, src2.cols * alpha, src2.rows));
		p2 = src1(Rect(src1.cols * alpha, 0, src1.cols * beta, src1.rows));
		hconcat(p1, p2, dst);
	}
	imshow("Linear Blend", dst);
}
int main(void)
{
	src1 = imread("red.jpg");
	src2 = imread("purple.jpg");
	if (src1.empty()) { cout << "Error loading src1 \n"; return -1; }
	if (src2.empty()) { cout << "Error loading src2 \n"; return -1; }
	alpha_slider = 0;

	namedWindow("Linear Blend", WINDOW_AUTOSIZE); // Create Window
	char TrackbarName[50];
	sprintf(TrackbarName, "Percent", alpha_slider_max);
	createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);
	on_trackbar(alpha_slider, 0);
	waitKey(0);
	return 0;
}