#include <iostream>
#include <cmath>
using namespace std;

class MyArrayParent {
protected:
	int capacity; // сколько памяти выделено?
	int count; // количество элементов - сколько памяти используем
	double* ptr; // массив
public:
	//конструкторы и деструктор
	MyArrayParent(int dimension = 100) {
		std::cout << "\nMyArray constructor";
		ptr = new double[dimension];
		capacity = dimension;
		count = 0;
	} //Конструктор класса, выделяющий память под массив заданной размерности dimension  инициализирующий поля capacity и count, а также указатель на массив ptr.

	MyArrayParent(const MyArrayParent& arr) {
		Copy(arr);
	} //Конструктор копирования, создающий копию переданного объекта arr

	//конструктор принимает существующий массив
	MyArrayParent(const double* arr, int len) {
		std::cout << "\nMyArray constructor";
		capacity = len;
		count = 0;
		//заполнить массив ptr, заполнить поля
		for (int i = 0; i < len; i++) {
			ptr[i] = arr[i];
			count++;
		}
	}//конструктор, который принимает существующий массив arr заданной длины len и копирует его в массив ptr.

	//деструктор
	virtual ~MyArrayParent() {
		std::cout << "\nMyArray destructor";
		if (ptr != NULL) {
			delete[] ptr;
			ptr = NULL;
		}
	} //освобождает выделенную память под массив ptr.

	//обращение к полям
	int Capacity() const { return capacity; }
	int Size() const { return count; }

	double GetComponent(int index) const {
		if (index >= 0 && index < count)
			return ptr[index];


		return -1;
	} //возвращает значение элемента массива по индексу index.

	void SetComponent(int index, double value) {
		if (index >= 0 && index < count)
			ptr[index] = value;

	} //устанавливает новое значение value для элемента массива по индексу index.

	//добавление в конец нового значения
	void push(double value) {
		if (count < capacity) {
			ptr[count] = value;
			count++;
		}
	} // добавляет новое значение value в конец массива.

	//удаление элемента с конца
	void RemoveLastValue() {
		if (count >= 0)
			count--;
	} //Удаляет последний элемент массива

	double& operator[](int index) {
		//перегрузка оператора []
		return ptr[index];
	} 

	void Copy(const MyArrayParent& V) {
		capacity = V.Capacity();
		for (int i = 0; i < V.Size(); i++) {
			ptr[i] = V.GetComponent(i);
			count++;
		}
	} //копирует содержимое объекта V в текущий объект.

	int IndexOf(int data) {
		for (int i = 0; i < count; i++) {
			if (ptr[i] == data) return i;
		}
		return -1; // не найдено
	} //возвращает индекс элемента data в массиве, если он присутствует.

	MyArrayParent& operator=(const MyArrayParent& V) {
		std::cout << "\noperator = ";

		Copy(V);

		return *this;
		//перегрузка оператора присваивания, копирует содержимое объекта V в текущий объект и возвращает ссылку на него.
	}

	/*MyArrayParent(const MyArrayParent& V) {
		std::cout << "\nCopy constructor";
		//создание копии объекта - в основном, при возвращении результата из функции / передаче параметров в функцию
	}*/

	void print() {
		std::cout << "\nMyArr, size: " << count << ", values: {";
		int i = 0;

		for (i = 0; i < count; i++) {
			std::cout << ptr[i];
			if (i != count - 1)
				std::cout << ", ";
		}
		std::cout << "}";
	}
};

class MyArrayChild : public MyArrayParent
{
public:
	//используем конструктор родителя.
	MyArrayChild(int Dimension = 100) : MyArrayParent(Dimension)
	{
		cout << "\nMyArrayChild constructor";
	}
	~MyArrayChild() { cout << "\nMyArrayChild destructor\n"; }
	//удаление элемента
	//void RemoveAt(int index = -1);
	void RemoveAt(int index = -1)
	{
		if (index < 0 || index >= count) return;

		for (int i = index; i < count - 1; i++)
			ptr[i] = ptr[i + 1];

		count--;
	}

	double Geo()
	{
		double sum = 1.0;
		for (int i = 0; i < count; i++) {
			sum = sum * ptr[i];
		}
		return pow(sum, 1.0 / count);
	}

	//поиск элемента
	//int IndexOf(double value, bool bFindFromStart = true);
	//вставка элемента
	void InsertAt(double value, int index = -1) {
		if (count >= capacity || index > count) return;
		if (index == -1) index = count;
		for (int i = count; i > index; i--)
			ptr[i] = ptr[i - 1];
		ptr[index] = value;
		count++;
	}
};

class MySortedArray : public MyArrayChild {
public:
	MySortedArray(int dimension = 100) {
		MyArrayParent();
	}

	double Geo()
	{
		double result = 1.0;
		for (int i = 0; i < count; i++)
		{
			result *= ptr[i];
		}

		// Возвращаем результат
		return pow(result, 1.0 / count);
	}

	int IndexOf(double data) {
		int i = count / 2;
		int step = i;

		bool is_save = true;
		int last_index = -1;

		while (ptr[i] != data) {
			if (i < 0 or i >= count) return -1;

			if (data > ptr[i])
				step = step / 2;
			if (step == 0) step = 1;
			else
				step = -(step / 2);
			if (step == 0) step = -1;


			if (i == last_index) return -1;
			if (is_save) last_index = i;
			is_save = !is_save;

			i += step;
		}

		return i;
	}

	void Swap(double* p1, double* p2) {
		double c = *p1;
		*p1 = *p2;
		*p2 = c;
	} //Метод Swap принимает указатели на два элемента массива и меняет их местами. Таким образом, происходит обмен значениями между двумя элементами путем использования временной переменной.

	void Sort() {
		if (count < 2) return;

		int sort = 0;

		while (sort < count - 1) {
			if (ptr[sort] > ptr[sort + 1]) {
				Swap(&ptr[sort], &ptr[sort + 1]);
				sort = 0;
			}
			else
				sort++;
		}
	} //Метод сортировки пузырьком

	void push(double value) {
		MyArrayParent::push(value);
		Sort();
	} //Метод push добавляет новый элемент в массив. Сначала вызывается метод push из родительского класса MyArrayParent для добавления нового элемента, а затем вызывается метод Sort() для пересортировки массива в случае необходимости, чтобы поддержать упорядоченность элементов.
};
int main()
{
	if (true)
	{
		MyArrayParent arr;
		int i = 0;
		for (i = 0; i < 10; i++)
		{
			arr.push(i + 1);
		}
		MyArrayParent p1;
		p1 = arr;
		p1.print();
		cout << "\n";

		MyArrayChild ch;
		for (int i = 0; i < 10; i++) {
			ch.InsertAt(i + 1, i);
		}
		ch.Geo();
	}
	MyArrayChild ch;
	for (int i = 0; i < 10; i++) {
		ch.InsertAt(i + 1, i);
	}
	ch.print();
	ch.Geo();
	ch.print();

	MySortedArray s;
	double a[] = { 3, 1, 8, 9, 0, 7, 14, 212 };
	for (int i = 0; i < (sizeof(a) / sizeof(a[0])); i++) {
		s.push(a[i]);
	}
	s.print();

	s.RemoveAt(3);
	double geoResult = s.Geo();
	s.print();

	return 0;
}
