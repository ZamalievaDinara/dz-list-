#include <iostream>
#include <iomanip> // нужен для std::setfill и std::setw
#include <string>

using namespace std;

const size_t MAX_DIGITS = 9;

namespace List
{
	// Элемент списка
	struct Elem
	{
		int   data = 0;
		Elem* prev = nullptr;
		Elem* next = nullptr;
	};

	// Возвращает указатель на последний элемент списка
	Elem* get_last_element(Elem* list)
	{
		Elem* last = list;
		while (last && last->next)
			last = last->next;
		return last;
	}

	// Добавляет в начало списка новый элемент
	void push_front(Elem*& list, int data)
	{
		auto* new_elem = new Elem;
		new_elem->data = data;

		// Если первый элемент не пустой указатель (не nullptr), то добавляем новый элемент перед ним
		if (list)
		{
			list->prev = new_elem;
			new_elem->next = list;
		}
		list = new_elem;
	}

	// Добавляет в конец списка новый элемент
	void push_back(Elem*& list, int data)
	{
		auto* new_elem = new Elem;
		new_elem->data = data;

		// Если последний элемент не пустой указатель (не nullptr), то добавляем новый элемент после него
		if (auto* last_elem = get_last_element(list))
		{
			last_elem->next = new_elem;
			new_elem->prev = last_elem;
		}
		else // т.к. последний элемент пустой указатель (nullptr), то список пуст и добавляем первый элемент
			list = new_elem;
	}

	// Очищает список (удаляет все его элементы)
	void clear(Elem*& list)
	{
		while (list)
		{
			auto* del_elem = list;
			list = del_elem->next;
			delete del_elem;
		}
	}

	// Выводит на экран значение элемента списка
	// При необходимости выводимое число дополняется нулями слева
	void print_element(Elem* list, Elem* element)
	{
		if (!element)
			return;

		if (element == list) // если это первый элемент списка, то выводим число
			cout << element->data;
		else // для остальных элментов списка - дополняем число нулями слева до ширины MAX_DIGITS
			cout << std::setfill('0') << std::setw(MAX_DIGITS) << element->data;
	}

	// Последовательно выводит на экран все элементы списка.
	// Поскольку элементы списка являются частью длинного числа, то при необходимости они дополняются нулями слева.
	// Для отладки можно задать разделитель
	void print(Elem* list, const string& splitter = "")
	{
		Elem* current = list;
		while (current)
		{
			if (!splitter.empty() && current != list)
				cout << splitter;
			print_element(list, current);
			current = current->next;
		}
		cout << endl;
	}
} // namespace List

// Проверяет является ли строка числом
bool is_number(std::string& str)
{
	if (str.empty())
		return false; // если строка пустая, то строка - не число

	for (char ch : str) // перебираем все символы строки
	{
		if (!std::isdigit(ch))
			return false; // если встретили не цифровой символ, то строка - не число
	}
	return true;
}

// Считывает число в виде строки из стандартного потока ввода
string input_number()
{
	string num;
	getline(std::cin, num);
	if (!is_number(num))
	{
		cout << "ОШИБКА: Введеная строка не является целым положительным числом." << endl;
		exit(-1);
	}
	num.erase(0, num.find_first_not_of('0')); // удаляем все нули в начале строки
	return num;
}

// Разделяет число в виде строки на куски размером SPLIT_SIZE и помещает их в список
// Возвращает указатель на начало списка
List::Elem* split_to_list(const string& num)
{
	List::Elem* list = nullptr;
	const size_t num_length = num.length();
	const size_t part_size = num_length % MAX_DIGITS; // вычисляем размер первого кусочка (может получиться 0)

	size_t from = 0;
	size_t to = part_size ? part_size : MAX_DIGITS;
	while (from < num_length)
	{
		string part = num.substr(from, to - from);
		List::push_back(list, std::stoi(part));
		from = to;
		to += MAX_DIGITS;
	}
	return list;
}

// Возвращает указатель на новый список, в котором записана сумма двух длинных чисел
List::Elem* sum_lists(List::Elem* list1, List::Elem* list2)
{
	List::Elem* result = nullptr;
	auto* cur1 = List::get_last_element(list1);
	auto* cur2 = List::get_last_element(list2);

	int max_sum = std::pow(10, MAX_DIGITS);
	int overflow = 0;
	while (cur1 || cur2)
	{
		int num1 = cur1 ? cur1->data : 0;
		int num2 = cur2 ? cur2->data : 0;
		int sum = num1 + num2 + overflow; // сумма двух чисел + переполнение от суммирования предыдущих частей

		List::push_front(result, sum % max_sum);
		overflow = sum / max_sum;

		cur1 = cur1 ? cur1->prev : nullptr;
		cur2 = cur2 ? cur2->prev : nullptr;
	}

	if (overflow)
		List::push_front(result, overflow);

	return result;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	cout << "Данная программа выполняет сложение длинных целых чисел." << endl;

	cout << "Введите первое целое число:" << endl;
	string str1 = input_number();
	auto* list1 = split_to_list(str1);

	cout << "Введите второе целое число:" << endl;
	string str2 = input_number();
	auto* list2 = split_to_list(str2);

	auto* result = sum_lists(list1, list2);

	List::print(list1, " ");
	List::print(list2, " ");
	List::print(result, " ");


	List::clear(list1);
	List::clear(list2);
	List::clear(result);
	return 0;
}