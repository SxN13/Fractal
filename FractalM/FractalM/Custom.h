#include <Windows.h>

void SCCP(int x, int y) //������ � �������
{
	COORD position;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	position.X = x;
	position.Y = y;

	SetConsoleCursorPosition(hConsole, position);
}

void SCP(int x, int y) //������ �� ������
{
	SetCursorPos(x, y);
}