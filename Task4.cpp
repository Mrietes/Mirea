#include <iostream>
#include <cctype>
#include <cstring>

using namespace std;

class BaseString
{
protected:
	char* p;
	int len;
	int capacity;
public:
	BaseString(char* ptr) //Конструктор примимает динамический массив, откуда надо скопировать информацию.
	{
		len = strlen(ptr) + 1;
		capacity = 256;
		p = new char[capacity];
		for (int i = 0;i < len;i++)
		{
			p[i] = ptr[i];
		}
		p[len] = '\0';
	}
	BaseString(int Capacity = 256) //Объем памяти, который надо выделить.
	{
		cout << "\nBase Constructor 0\n";
		capacity = Capacity;
		p = new char[capacity];
		len = 0;
	}
	~BaseString() //Дестркутор   
	{
		if (p != NULL)
			delete[] p;
		len = 0;
	}
	int Length() { return len; } //Возвращает длину строки
	int Capacity() { return capacity; } //Возвращает емкость строки

	char& operator[](int i) { return p[i]; } //Перегружает оператор доступа к элементам массива для строки.

	BaseString& operator=(BaseString& s) // Копирует содержимое строки из переданного объекта в текущий объект.
	{
		len = s.Length();
		p = new char[s.capacity];
		capacity = s.capacity;
		for (int i = 0;i < s.Length();i++)
		{
			p[i] = s[i];
		}
		p[len - 1] = '\0';
		return *this;
	}

	BaseString(BaseString& s) //Конструктор копирования, копирующий содержимое переданного объекта в текущий объект.
	{
		len = s.Length();
		p = new char[s.capacity];
		capacity = s.capacity;
		for (int i = 0;i < s.Length() - 1; i++)
		{
			p[i] = s[i];
		}
		p[len - 1] = '\0';
	}
	virtual void print()
	{
		int i = 0;
		while (p[i] != '\0') { cout << p[i]; i++; }
	}
};

class String : public BaseString
{ public:

	String(char* ptr) : BaseString(ptr) {}

	String(int Capacity = 256) : BaseString(Capacity) {}

	String(String& s) : BaseString(s) {}

	void toupper()
	{
		for (int i = 0; i < len - 1; ++i)
		{
			p[i] = std::toupper(p[i]);
		}
	}
};

int main()
{
	char text[] = "Hello World!";
	String str(text);
	str.toupper();
	str.print();

	return 0;
}