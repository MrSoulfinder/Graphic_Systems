#include "medianFilter.h"


void MedianFilter::filterImage(int _window, std::vector<unsigned int>& _weights, bitmap_image* _image)
{
	window.x = _window; 
	window.y = _window;
	image = _image; 

	offset.x = window.x / 2; 
	offset.y = window.y / 2;

	linearWindowSize = window.x + window.y - 1; 
	weights = &_weights; 

	copyImage();
	
	for (int x = offset.x; x < image->width() - offset.x; x++)
	{
		for (int y = offset.y; y < image->height() - offset.y; y++)
		{
			windowValues.clear();

			//������ ������� ��� ������
			int pixelIndex = 0;
			//������� ��� �
			int startY, endY;
			bitmap_image::rgb_t pixel;
			for (int wX = x - offset.x; wX <= x + offset.y; wX++) // �������� ���������� �������� � ���� �����-�������
			{
				if (wX == x)
				{
					startY = y - offset.y; // ������ ������� ��� y � �������������� ����
					endY = y + offset.y;
				}
				else
				{
					startY = endY = y;
				}
				for (int wY = startY; wY <= endY; wY++)  // �������� ���������� �������� � ���� ������-����
				{
					pixel = copy.get_pixel(wX, wY); // �������� ������� �� �����������
					for (int i = 0; i < weights->at(pixelIndex); i++)
						windowValues.push_back(pixel); // ��������� �������� ������� 
					pixelIndex++;
				}
			}
			
			if (pixelIndex > 0)
			{
				std::sort(windowValues.begin(), windowValues.end(), [](bitmap_image::rgb_t a, bitmap_image::rgb_t b) {
					return a.red > b.red;
					}); // ��������� ������� 
				pixel.red = windowValues[windowValues.size() / 2].red;

				std::sort(windowValues.begin(), windowValues.end(), [](bitmap_image::rgb_t a, bitmap_image::rgb_t b) {
					return a.green > b.green;
					}); // ��������� �������
				pixel.green = windowValues[windowValues.size() / 2].green;

				std::sort(windowValues.begin(), windowValues.end(), [](bitmap_image::rgb_t a, bitmap_image::rgb_t b) {
					return a.blue > b.blue;
					}); // ��������� �����
				pixel.blue = windowValues[windowValues.size() / 2].blue;

				image->set_pixel(x, y, pixel); // ��������� ����� ������� � �����������
			}

		}
	}
}

void MedianFilter::copyImage()
{
	copy = *image;
}

void AddNoise(bitmap_image* image, double probability) // ������� ����
{

	int size = image->width() * image->height();
	int count = (int)(size * probability) / 100;
	int x, y;
	for (int i = 0; i < count; i++)
	{
		x = rand() % image->width();
		y = rand() % image->height();
		image->set_pixel(x, y, rand()%255, rand()%255, rand()%255);
	}
}

