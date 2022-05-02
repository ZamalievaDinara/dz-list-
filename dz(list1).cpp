using namespace std;

#include <iostream>
#include <string>

struct Elem
{
    /// Поле для хранения данных
    int a;

    /// Указатель на следующий элемент.
    Elem* next = nullptr;
};

Elem* search(Elem* first, int data)
{
    Elem* cur = first;
    while (cur)
    {
        if (cur->a == data)
            return cur;
        cur = cur->next;
    }
    return cur;
}

void add(Elem*& first, int data)
{   
    // Ищем элемент в списке
    if (search(first, data) != nullptr)
        return;
    
    // Если элемент не найден, то создаем новый элемент и добавляем в список
    Elem* newel = new Elem;
    newel->a = data;

    Elem* curr = first;
    // Пока существует следующий элемент в списке
    while (curr)
    {
        if (!curr->next)
        {
            curr->next = newel;
            return;
        }
        // Перемещаем указатель на следующий элемент
        curr = curr->next;
    }
    first = newel;
}

/// <summary>
/// Отображение списка на экране
/// </summary>
/// <param name="list">Список, содержимое которого нужно показать</param>
void show_list(Elem* list)
{
    Elem* curr = list;
    if (!curr) cout << "Список пуст";
    else while (curr)
    {
        cout << curr->a << (curr->next ? " " : "\n");
        curr = curr->next;
    }
}

void clear(Elem*& first)
{
    while (first)
    {
        // Запоминаем элемент для удаления
        Elem* del = first;
        // Начало списка смещаем на следующий элемент                   
        first = first->next;
        // Удаляем сохраненный элемент.                
        delete del;
    }
}

void main()
{
    setlocale(LC_ALL, "rus");

    // Указатель на начало списка (изначально пуст)
    Elem* list = nullptr;

    cout << "Введите количество элементов списка: ";
    int count;
    cin >> count;
    // cout << endl;

    for (int i = 0; i < count; i++)
    {
        cout << "Введите " << i + 1 << "-ый элемент списка: ";
        int data;
        cin >> data;
        add(list, data);
    }

    cout << endl << "Элементы списка:" << endl;
    show_list(list);

    clear(list);
}
