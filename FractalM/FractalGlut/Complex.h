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
	double abs() { return r * r + i * i; }
};