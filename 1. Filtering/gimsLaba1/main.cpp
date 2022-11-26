#include <iostream>
#include <string>
#include <vector>
#include "bitmap-image.hpp"
#include "medianFilter.h"

const int filterSize = 3;
// 1 строка - горизонтальна€ часть окна, 2 строка - вертикальна€
unsigned long matrixWeight[][filterSize] = { {2,1,2},
											 {2,1,2} };

std::vector<unsigned int> matrixToVector(int rows, int columns)
{
	std::vector<unsigned int> output;
	for (int x = 0; x < columns / 2; x++)
	{
		output.push_back(matrixWeight[0][x]);
	}
	for (int y = 0; y < columns; y++)
	{
		output.push_back(matrixWeight[1][y]);
	}
	for (int x = columns / 2 + 1; x < columns; x++)
	{
		output.push_back(matrixWeight[0][x]);
	}
	return output;
}

int main(int argc, char* argv[])
{
	const std::string fileName = "Dune.bmp"; 
	bitmap_image* image = new bitmap_image(fileName);

	AddNoise(image, 20); // ƒобавл€ем 20% шума

	image->save_image("s_shumom_" + fileName); 
	
	MedianFilter filt; // Ёкземл€р фильтра
	std::vector<unsigned int> vectorWeights = matrixToVector(3, filterSize);
	filt.filterImage(3, vectorWeights, image); 

	image->save_image("s_filtrom_" + fileName); 

	delete image;
	system("pause");
	return 0;
}