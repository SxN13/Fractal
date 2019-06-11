#include <cmath>
#include <complex>
#include <iostream>

#include "glut.h"
#include "Complex.h"

Complex cMin(-1., -1.), cMax(1., 1.);
Complex cm(1., 0.);

int iMS(Complex z, Complex c0, int it, int count)
{
	if ((cMin > z) && (z < cMax) && (count < it))
	{
		iMS((z*z + c0), c0, it, count + 1);
	}
	else
	{
		return count;
	}
}

Complex map(int x, int y, double scale, int mx, int my)
{
	int fx = x * scale + mx;
	int fy = y * scale + my;
	Complex c(fx, fy);
	return c;
}

void img(double s, double mx, double my, int it, int WW, int WH) 
{
	Complex z(0, 0);
	int count = NULL;
	for (size_t x = 0; x < WW; x++)
	{
		for (size_t y = 0; y < WH; y++)
		{
			count = iMS(z, map(x, y, s, mx, my), it, 0);
			if (count == it) {
				std::cout << "not\n";
			}
			else
			{
				std::cout << "yes\n";
			}
		}
	}
}

int main()
{
	Complex z(0, 0);
	
	img(1., 0., 0., 20, 1000, 1000);

	system("pause");
	return 0;
}