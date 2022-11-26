#ifndef _MEDIAN_FILTER_
#define _MEDIAN_FILTER_

#include "bitmap-image.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

class MedianFilter // ����� ��������
{
public:
	MedianFilter() { }
	struct point2 { // ��������� �����
		int x;
		int y;
	};

	bitmap_image* image; // �����������
	bitmap_image copy;
	point2 window, offset; // ������ ���� � ����� ����������� ������ 
	std::vector<bitmap_image::rgb_t> windowValues; // ������ � ����
	std::vector<unsigned int>* weights; 
	int linearWindowSize; // �������� ������ ����

	void filterImage(int _window, std::vector<unsigned int>& _weights, bitmap_image* _image);
	void copyImage();
};

void AddNoise(bitmap_image* image, double probability);
#endif // !_MEDIAN_FILTER_
