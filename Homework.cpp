#include <iostream>
using namespace std;

class Fraction
{
private:
	int numerator; //���������
	int denominator; //�����������
public:

	double number = 0.5;

	Fraction(int n = 0, int d = 1) // ����������� ������ 
	{
		numerator = n;
		denominator = d;
	}
	void changeFraction() //����� ��������� � �����������
	{
		int temp = numerator;
		numerator = denominator;
		denominator = temp;
	}
	bool operator==(const Fraction& other) const {
		return (numerator * other.denominator == other.numerator * denominator);
	}

	bool operator<(const Fraction& other) const {
		return (numerator * other.denominator < other.numerator * denominator);
	}

	bool operator>(const Fraction& other) const {
		return (numerator * other.denominator > other.numerator * denominator);
	}

	bool operator < (double number) const
	{
		return static_cast<int>(numerator) / denominator < number; //��������� ����� � ������������� �����
	}
	void Print1()
	{
		cout << numerator << "/" << denominator << endl;
	}
	void Print2()
	{
		if ((numerator / denominator) > number)
		{
			cout << numerator << "/" << denominator << " > " << number << endl;
		}
		if ((numerator / denominator) < number)
		{
			cout << numerator << "/" << denominator << " < " << number << endl;
		}
		if ((numerator / denominator) == number)
		{
			cout << numerator << "/" << denominator << " = " << number << endl;
		}
	}
};


int main()
{
	setlocale(LC_ALL, "Russian");
	Fraction f1(3, 4);
	Fraction f2(2, 3);


	cout << "����� �� �������� ����� ��������� � �����������: ";
	f1.Print1();
	f2.Print1();
	f1.changeFraction();
	cout << "����� ����� �����: ";
	f1.Print1();
	
	f1.Print2(); //��������� ����� � ������������ ������

	if (f1 == f2) {
		cout << "����� �����" << endl;
	}
	else if (f1 < f2) {
		cout << "����� f1 ������ ����� f2" << endl;
	}
	else {
		cout << "����� f1 ������ ����� f2" << endl;
	}
	return 0;
}


