using System;
using System.IO;
using System.Drawing;

namespace gimsLab2ol
{
    class Program
    {
        static void Main(string[] args)
        {
            //полный путь к папке
            string path = Path.GetFullPath("./");
            Console.WriteLine(path);

            //название файла
            string image = "Ducky.bmp";
            MaskBorder border = new MaskBorder();
            Bitmap bitmap = new Bitmap(path + image);

            border.Image = bitmap;

            //выполняем создание массива F'
            border.MakeMap();

            for (; ; )
            {
                Console.WriteLine("Min F: " + border.FMin);
                Console.WriteLine("Max F: " + border.FMax);
                Console.Write("Enter Porog: ");
                
                //вводим порог
                border.Porog = System.Convert.ToInt32(Console.ReadLine());
              
                //сохраняем итоговое изображение
                border.Save(path + "contur_" + image);
                Console.Write("Press Q to quit, any other to continue");
                ConsoleKeyInfo key = Console.ReadKey();
                if (key.Key == ConsoleKey.Q)
                    break;
                Console.Clear();
            }
        }
    }
}
