using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace gimsLab2ol
{
    class MaskBorder
    {
        //курсовая маска юго-запад
        public int[,] southWest = { { -1,  1,  1 }, 
                                    { -1,  2,  1 },
                                    { -1, -1, -1 } };
        //порог
        public int Porog { get; set; }
        public Bitmap Image { get; set; }
        //минимальное и максимальное значение Fmix, Fmax
        public int FMin { get; protected set; }
        public int FMax { get; protected set; }

        //двумерный массив яркости каждого пикселя изображения
        private int[,] brightnessMap;
        //массив F'
        private int[,] fMap;
        //выходное изображение
        private Bitmap outputImage;

        public void MakeMap()
        {
            if (Image == null)
                return;

            //создаем и заполняем массив яркости
            MakeBrightnessMap();
            outputImage = new Bitmap(Image);

            //создаем массив F'
            fMap = new int[Image.Width, Image.Height];

            //проходим все изображение кроме краев
            for (int x = 1; x < Image.Width - 1; x++)
                for (int y = 1; y < Image.Height - 1; y++)
                {
                    int F = 0;
                    //проходим по маске, мх и му координаты маски
                    //х + мх - 1 координаты в изображении
                    for (int mx = 0; mx < 3; mx++)
                        for (int my = 0; my < 3; my++)
                        {
                            F += brightnessMap[x + mx - 1, y + my - 1] * southWest[mx, my];
                        }
                    
                    //записаываем F' в массив
                    fMap[x, y] = F;
                    if (F > FMax)
                        FMax = F;
                    if (F < FMin)
                        FMin = F;
                }
        }

        public void Save(string fileName)
        {
            for (int x = 1; x < Image.Width - 1; x++)
                for (int y = 1; y < Image.Height - 1; y++)
                {
                    Color pixel;
                    //если больше порога, ставит белый пиксель, если меньше то черный
                    if (fMap[x, y] > Porog)
                        pixel = Color.White;
                    else
                        pixel = Color.Black;
                    outputImage.SetPixel(x, y, pixel);
                }
            //сохраняем результат
            outputImage.Save(fileName);
        }

        private void MakeBrightnessMap()
        {
            brightnessMap = new int[Image.Width, Image.Height];
            for (int x = 0; x < Image.Width; x++)
                for (int y = 0; y < Image.Height; y++)
                {
                    //получаем исходный пиксель изображения
                    Color pixel = Image.GetPixel(x, y);
                    //переводим пиксель в яркость по формуле
                    brightnessMap[x, y] = (int)(0.299 * pixel.R +
                                                0.587 * pixel.G +
                                                0.144 * pixel.B);
                }
        }
    }
}
