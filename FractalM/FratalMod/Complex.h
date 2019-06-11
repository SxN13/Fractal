#pragma once
class Complex //класс комплексных чисел, необходимый для расчетов
{
private:
	double r, i;
public:
	Complex(double r, double i)
	{
		this->r = r; this->i = i;
	}
	Complex operator+(const Complex &c)
	{
		return Complex(r + c.r, i + c.i);
	}
	Complex operator*(const Complex &c)
	{
		return Complex(r * c.r - i * c.i, 2 * r * c.i);
	}
	bool operator<(const Complex &c)	//почему-то не работает!
	{
		if ((r < c.r) && ((r == c.r) | (i < c.i)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator>(const Complex &c)
	{
		if ((r > c.r) && ((r == c.r) | (i > c.i)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	double abs() { return r * r + i * i; }
};