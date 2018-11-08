#include <fstream>
#include <iostream>

#include "Custom.h"
#include <omp.h>

using namespace std;

int findMandelbrot(double cr, double ci, double max_iterations) {
	int i = 0;
	double zr = 0.0, zi = 0.0;
	while (i < max_iterations && zr * zr + zi * zi < 4.0)

	{
		double temp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = temp;
		i++;
	}
	return i;

}

double mapToReal(int x, int imageWidth, double minR, double maxR)
{
	double range = maxR - minR;
	return x * (range / imageWidth) + minR;

}
double mapToImaginary(int y, int imageHeight, double minI, double maxI)
{
	double range = maxI - minI;
	return y * (range / imageHeight) + minI;

}

/*  512 512 256 -1.5 0.7 -1.0 1.0  */
int main()
{
	int imageWidth, imageHeight, maxN;
	double minR, maxR, minI, maxI;

	imageWidth = 20000;
	imageHeight = 20000;
	maxN = 256;

	minR = -1.5;
	maxR = 0.7;

	minI = -1.0;
	maxI = 1.0;

	ofstream fout("output_image.ppm"); //открывает файл
	fout << "P3" << endl;
	fout << imageWidth << " " << imageHeight << endl; //размеры
	fout << "256" << endl;  //максимально число пикслелей в ргб

	int iter = imageWidth * imageHeight;
	int counterIter = 0;
	float pr = 0;

	for (int y = 0; y < imageHeight; y++) //строки
	{
		#pragma omp for ordered schedule(dynamic)
		for (int x = 0; x < imageWidth; x++) //пиксели в строке
		{
			double cr = mapToReal(x, imageWidth, minR, maxR);
			double ci = mapToImaginary(y, imageHeight, minI, maxI);

			int n = findMandelbrot(cr, ci, maxN);

			int r = ((int)(n * 60) % 256);
			int g = ((n * 30) % 256);
			int b = ((n * 10) % 256);
	#pragma omp ordered
			fout << r << "  " << g << "  " << b << "  ";

			}
		fout << endl;

	}
	fout.close();
	cout << "Finished " << endl;

	system("pause");
	return 0;
}