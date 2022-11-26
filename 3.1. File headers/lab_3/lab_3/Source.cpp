#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <string>

// bmp_editor.cpp : Defines the entry point for the console application.

using namespace std;

int isChanged = 0;
double fraction = 0;

#pragma pack(2)

//Заголовок файла BMP 
typedef struct tBITMAPFILEHEADER
{
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
}sFileHead;

//Заголовок BitMap's
typedef struct tBITMAPINFOHEADER
{
	DWORD	rastrSize;
	LONG	biWidth;
	LONG	biHeight;
	WORD	biPlanes;
	WORD	biBitCount;
	DWORD	biCompression;
	DWORD	biSizeImage;
	LONG	biXPelsPerMeter;
	LONG	biYPelsPerMeter;
	DWORD	biClrUsed;
	DWORD	biClrImportant;
}sInfoHead;

typedef struct tMYBITMAPHEADER {
	WORD	bfType;
	char	comment[19] = { "by Chernookiy" };
	WORD	version;
	DWORD	rastrSize;
	DWORD	PictureSize;
	WORD	bfOffBits;
	DWORD	biWidth;
}sMyHead;

sFileHead FileHead;
sInfoHead InfoHead;
sMyHead MyHead;

//Пиксель
struct Color
{
	BYTE blue;
	BYTE green;
	BYTE red;

	Color() = default;

	Color(BYTE red_, BYTE green_, BYTE blue_) {
		blue = blue_;
		green = green_;
		red = red_;
	}

	Color* operator=(const Color& source) {
		this->blue = source.blue;
		this->green = source.green;
		this->red = source.red;
		return this;
	}
};

//Размер 1-го пикселя
int pixel_size = sizeof(Color);


//1 - BMP, 2 - CMP
int img_type = 0;

//Исходное изображение
Color* rawImage = nullptr;
Color* rawImage1 = nullptr;
Color* rawImage2 = nullptr;
//Результативное изображение
Color* modifiedImage = nullptr;

//Размер изображения
int width = 0;
int height = 0;

//Вывести заголовок BMP файла
void ShowBMPHeaders(tBITMAPFILEHEADER fh, tBITMAPINFOHEADER ih)
{
	cout << "\n-----------------------------------\n";
	cout << "Type: " << (CHAR)fh.bfType << endl;
	cout << "Size: " << fh.bfSize << endl;
	cout << "Shift of bits: " << fh.bfOffBits << endl;
	cout << "Width: " << ih.biWidth << endl;
	cout << "Height: " << ih.biHeight << endl;
	cout << "Planes: " << ih.biPlanes << endl;
	cout << "BitCount: " << ih.biBitCount << endl;
	cout << "Compression: " << ih.biCompression;
	cout << "\n-----------------------------------\n";
}

//Функция для загрузки изображения
bool OpenImage(const string& path)
{
	ifstream img_file;
	Color temp;
	char buf[3];

	//Открыть файл на чтение
	img_file.open(path.c_str(), ios::in | ios::binary);
	if (!img_file)
	{
		cout << "File isn`t open!" << endl;
		return false;
	}

	//Считать заголовки BMP
	img_file.read((char*)&FileHead, sizeof(FileHead));
	img_file.read((char*)&InfoHead, sizeof(InfoHead));

	img_type = 1;
	ShowBMPHeaders(FileHead, InfoHead);
	
	//Присвоить длину и ширину изображения
	width = InfoHead.biWidth;
	height = InfoHead.biHeight;


	//Выделить место под изображение
	rawImage = new Color[width * height];

	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_file.read((char*)&temp, pixel_size);
			rawImage[i * width + j] = temp;
		}
		//Дочитать биты используемые для выравнивания до двойного слова
		img_file.read((char*)buf, j % 4);
	}
	img_file.close();

	return true;
}

//Функция для сохранения изображение
bool SaveImage(const string& path)
{
	ofstream img_file;
	char buf[3];

	//Открыть файл на запись
	img_file.open(path.c_str(), ios::out | ios::binary);
	if (!img_file)
	{
		return false;
	}

	img_file.write((char*)&FileHead, sizeof(FileHead));
	img_file.write((char*)&InfoHead, sizeof(InfoHead));

	//Скопировать из исходного в результирующее изображение
	/*modifiedImage = new Color[width * height];
	memcpy(modifiedImage, rawImage, width * height * sizeof(Color));*/

	//Записать файл
	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_file.write((char*)&modifiedImage[i * width + j], pixel_size);
		}
		img_file.write((char*)buf, j % 4);
	}
	img_file.close();

	return true;
}

//Скопировать содержимое результируещего изображения в начальное
void CopyDstToSrc()
{
	if (modifiedImage != 0)
	{
		memcpy(rawImage, modifiedImage, width * height * sizeof(Color));
	}
}

//Отобразить текущее изображение с помощью вызова стандартного просмотрщика
void ShowImage(string path)
{
	system(path.c_str());
}

//Считать путь к изображению
void ReadPath(string& str)
{
	str.clear();
	cout << "Enter path and name of ";
	switch (isChanged)
	{
	case 0:
		cout << "raw image\n";
		break;
	}
	cin >> str;
}


void ClearMemory(void) {
	//Освободить память исходного изображения
	if (rawImage != nullptr)
	{
		delete[] rawImage;
	}
	//Освободить память результрующего изображения
	if (modifiedImage != nullptr)
	{
		delete[] modifiedImage;
	}
}


bool saveMyFormat(const string& path);
bool openMyFormat(const string& path);

int main() {

	//Путь к текущему изображению
	string pathToRawImg, pathToNoisedImg, pathToRestoredImg, pathToConstastImg;
	int probability = 0;

	ReadPath(pathToRawImg);   //&pathToRawImg

	if (!OpenImage(pathToRawImg)) {	//открыть начальное изображение
		ClearMemory();
		return EXIT_FAILURE;
	};

	cout << "\t<<<<< Saving >>>>>";
	saveMyFormat("lab_3.CIV");
	cout << "\t<<<<< Open >>>>>";
	openMyFormat("lab_3.CIV");

}

bool saveMyFormat(const string& path) {
	ofstream img_file;
	char buf[3];

	//Открыть файл на запись
	img_file.open(path.c_str(), ios::out | ios::binary);
	if (!img_file)
	{
		return false;
	}

	MyHead.bfType = FileHead.bfType;
	MyHead.version = 1;
	MyHead.bfOffBits = sizeof(MyHead) + 1;
	MyHead.biWidth = InfoHead.biWidth;
	MyHead.PictureSize = width * height;
	MyHead.rastrSize = MyHead.PictureSize * 8 / InfoHead.biBitCount;

	cout << "\n-----------------------------------\n";
	cout << "Type: " << (CHAR)MyHead.bfType << endl;
	cout << "Comment: " << MyHead.comment << endl;
	cout << "Version: " << to_string(MyHead.version) << endl;
	cout << "Raster size: " << MyHead.rastrSize << endl;
	cout << "Picture size: " << MyHead.PictureSize << endl;
	cout << "Shift of bits: " << MyHead.bfOffBits << endl;
	cout << "Width: " << MyHead.biWidth << endl;
	cout << "\n-----------------------------------\n";

	img_file.write((char*)&MyHead, sizeof(MyHead));

	//Скопировать из исходного в результирующее изображение
	modifiedImage = new Color[width * height];
	memcpy(modifiedImage, rawImage, width * height * sizeof(Color));

	//Записать файл
	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_file.write((char*)&modifiedImage[i * width + j], pixel_size);
		}
		img_file.write((char*)buf, j % 4);
	}
	img_file.close();

	return true;
}

bool openMyFormat(const string& path) {
	ifstream img_file;
	Color temp;
	char buf[3];

	//Открыть файл на чтение
	img_file.open(path.c_str(), ios::in | ios::binary);
	if (!img_file)
	{
		cout << "File isn`t open!" << endl;
		return false;
	}

	//Считать заголовки BMP
	img_file.read((char*)&MyHead, sizeof(MyHead));

	img_type = 1;

	//Выделить место под изображение
	rawImage = new Color[width * height];

	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_file.read((char*)&temp, pixel_size);
			rawImage[i * width + j] = temp;
		}
		//Дочитать биты используемые для выравнивания до двойного слова
		img_file.read((char*)buf, j % 4);
	}
	img_file.close();

	cout << "\n-----------------------------------\n";
	cout << "Type: " << (CHAR)MyHead.bfType << endl;
	cout << "Comment: " << MyHead.comment << endl;
	cout << "Version: " << to_string(MyHead.version) << endl;
	cout << "Raster size: " << MyHead.rastrSize << endl;
	cout << "Picture size: " << MyHead.PictureSize << endl;
	cout << "Shift of bits: " << MyHead.bfOffBits << endl;
	cout << "Width: " << MyHead.biWidth << endl;
	cout << "\n-----------------------------------\n";

	return true;
}