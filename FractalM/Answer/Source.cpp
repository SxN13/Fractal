#include <math.h>
#include <iostream>
#include <ctime>

int main()
{
	srand(time(NULL));

	std::cout << (rand() % rand()) % 100 << "\n";

	system("pause");
}