#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;


template<class T>
class Element //Класс представляет собой элемент списка 
{ 
protected:
	Element* prev;
	Element* next;
	T info; //поле info типа T для хранения информации.
public:
	void setPrev(Element<T>* p)
	{
		prev = p;
	}

	void setNext(Element<T>* n)
	{
		next = n;
	}

	void setInfo(T info)
	{
		info = info;
	}

	Element<T>* getPrev() 
	{
		return prev;
	}

	Element<T>* getNext()
	{
		return next;
	}

	T getInfo()
	{
		return info;
	}

	Element(T data) //Конструктор класса, который принимает параметр типа T и инициализирует поля класса.
	{
		next = prev = NULL; //Устанавливает значения полей next и prev в NULL
		info = data; //Инициализирует поле info переданным значением data
	}

	Element(const Element* Next, const Element* Prev, const T data) //Конструктор класса, принимающий указатели на следующий и предыдущий элементы типа Element, а также параметр типа T.
	{
		next = Next;
		prev = Prev;
		info = data;
	}
	Element(const Element& el) //Конструктор копирования
	{
		next = el.next;
		prev = el.prev;
		info = el.info;
	}

	template<class T1>
	friend ostream& operator<<(ostream& s, Element<T1>& el); //Объявление дружественной функции перегрузки оператора вывода для класса Element. Это позволит использовать эту функцию для вывода объектов класса Element.
};

template<class T1>
ostream& operator<<(ostream& s, Element<T1>& el)  //Определение оператора вывода для объектов класса Element.В данном случае выводится содержимое поля info объекта el.
{
	s << el.info;
	return s;
}

template<class T>
class LinkedList // Связный список
{
protected:
	Element<T>* head; //Указатели head и tail на элементы типа Element
	Element<T>* tail;
	int count; //Счетчик  count для отслеживания количества элементов в списке.
public:
	LinkedList() //Конструктор класса LinkedList, инициализирующий указатели head и tail в NULL и счетчик count в 0.
	{
		head = tail = NULL;
		count = 0;
	}

	virtual ~LinkedList()
	{
		Element<T>* a = head;
		Element<T>* b = head;
		for (; a != NULL; a = b)
		{
			b = a->getNext();
			delete a;
		}
		count = 0;
	}

	virtual Element<T>* pop() = 0; //Виртуальный метод pop(). Он используется для извлечения элемента из списка.

	virtual Element<T>* push(const T value) = 0; //Виртуальный метод push(). Он используется для добавления элемента в список.

	virtual Element<T>& operator[](int index) //Виртуальный метод оператора индексации []. Он позволяет получить доступ к элементу по индексу.
	{
		Element<T>* current = head;

		for (int i = 0; current != NULL && i < index; current = current->getNext(), i++) {}
		return *current;
	}
	virtual bool isEmpty() { return (LinkedList<T>::count == 0); } //Виртуальный метод isEmpty(), который возвращает true, если список пустой (количество элементов равно 0), иначе false.

	template<class T1>
	friend ostream& operator<<(ostream & s, LinkedList<T1>&el);

};
template<class T1>
ostream& operator<<(ostream& s, LinkedList<T1>& el)
{
	Element<T1>* current;

	for (current = el.head; current != NULL; current = current->getNext())
	{
		s << *current << "; ";
	}
	return s;
}


class Vector
{
protected:
	int dimension;
	vector<double> coefficients;

public:
	Vector() : dimension(0) {}
	Vector(int dim, const vector<double>& coeffs) : dimension(dim), coefficients(coeffs) {}

	int getDimension() const { return dimension; }

	vector<double> getCoefficients() const { return coefficients; }

	double length() const // Вычисление длины вектора
	{
		double sum = 0;
		for (double coef: coefficients)
		{
			sum += coef * coef;
		}
		return sqrt(sum);
	}

	double dotProduct(const Vector& other) const // Скалярное произведение двух векторов
	{
		double result = 0;
		vector<double> otherCoeffs = other.getCoefficients();
		for (size_t i = 0; i < coefficients.size(); ++i)
		{
			result += coefficients[i] * otherCoeffs[i];
		}
		return result;
	}

	friend ostream& operator<<(ostream& os, const Vector& v) // Перегрузка оператора вывода для класса Vector
	{
		os << "(";
		for (size_t i = 0; i < v.coefficients.size(); ++i)
		{
			os << v.coefficients[i];
			if (i < v.coefficients.size() - 1)
				os << ", ";
		}
		os << ")";
		return os;
	}
};

class VectorElement : public Element<Vector>
{
public:
	VectorElement(Vector data): Element<Vector>(data) {}
};

class LinkedVector : public  LinkedList<Vector>
{
public: 
	  LinkedVector() : LinkedList<Vector>() {}

	  Element<Vector>* pop() {
		  if (LinkedList<Vector>::tail == NULL) {
			  cout << "List is empty." << endl;
			  return NULL;
		  }
		  Element<Vector>* temp = LinkedList<Vector>::tail;
		  if (LinkedList<Vector>::head == LinkedList<Vector>::tail) {
			  LinkedList<Vector>::head = LinkedList<Vector>::tail = NULL;
		  }
		  else {
			  LinkedList<Vector>::tail = LinkedList<Vector>::tail->getPrev();
			  LinkedList<Vector>::tail->setNext(NULL);
		  }
		  LinkedList<Vector>::count--;
		  return temp;
	  }

	  Element<Vector>* push(const Vector value) {
		  Element<Vector>* temp = new VectorElement(value);
		  if (LinkedList<Vector>::head == NULL) {
			  LinkedList<Vector>::head = LinkedList<Vector>::tail = temp;
		  }
		  else {
			  LinkedList<Vector>::tail->setNext(temp);
			  temp->setPrev(LinkedList<Vector>::tail);
			  LinkedList<Vector>::tail = temp;
		  }
		  LinkedList<Vector>::count++;
		  return temp;
	  }

	  void insert(const Vector value, int index) {
		  if (index < 0 || index > LinkedList<Vector>::count) {
			  cout << "Invalid index." << endl;
			  return;
		  }

		  Element<Vector>* temp = LinkedList<Vector>::head;
		  for (int i = 0; i < index; i++) {
			  temp = temp->getNext();
		  }

		  Element<Vector>* newElement = new VectorElement(value);
		  Element<Vector>* prevElement = temp->getPrev();

		  if (prevElement != NULL) {
			  prevElement->setNext(newElement);
			  newElement->setPrev(prevElement);
		  }
		  else {
			  LinkedList<Vector>::head = newElement;
		  }

		  newElement->setNext(temp);
		  temp->setPrev(newElement);
		  LinkedList<Vector>::count++;
	  }

	  void remove() {
		  Element<Vector>* temp = LinkedList<Vector>::tail;
		  LinkedList<Vector>::tail = temp->getPrev();
		  if (LinkedList<Vector>::tail == NULL) {
			  LinkedList<Vector>::head = NULL;
		  }
		  else {
			  LinkedList<Vector>::tail->setNext(NULL);
		  }
		  delete temp;
		  LinkedList<Vector>::count--;
	  }

};


int main()
{
	vector<double> coeffs1 = { 1.0, 2.0, 3.0 };
	Vector vec1(3, coeffs1);

	vector<double> coeffs2 = { 4.0, 5.0, 6.0 };
	Vector vec2(3, coeffs2);
	LinkedVector vecList;

	vecList.push(vec1);
	vecList.push(vec2);


	cout << "List of Vectors: " << vecList << endl;

	vecList.insert(Vector(3, { 7.0, 8.0, 9.0 }), 1);
	cout << "List after insertion: " << vecList << endl;

	vecList.remove();
	cout << "List after removal: " << vecList << endl;

	return 0;
}




