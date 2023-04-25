#include <iostream>
#include <iomanip>
#include <queue>
#include <cctype>
#include "fifo.h"

using namespace std;

int isCommand(char ch);           // Проверка на валидность команды пакета
int charToInt(char ch, bool x16); // Перевод шестнадцатеричного числа, записанного в char, в int

int main()
{
    constexpr std::uint8_t len = 128;
    using DataType = char;
    using SizeType = uint8_t;

    fifo::internal::FifoRaw<DataType, SizeType, len> commands;
    fifo::internal::FifoRaw<DataType, SizeType, len> result;

    // queue<char> commands; // Очередь для команд
    // queue<char> result; // Очередь для результата

    int currentChecksum = 0; // Значение контрольной суммы текущего пакета
    int checksum = 0;        // Контрольное значение пакета, которое было указано заранее

    // Инициализация пакета
    commands.pushSafe('d');
    commands.pushSafe('#');
    commands.pushSafe('$');
    commands.pushSafe('d');
    commands.pushSafe('$');
    commands.pushSafe('d');
    commands.pushSafe('2');
    commands.pushSafe('#');
    commands.pushSafe('9');
    commands.pushSafe('6');
    // cout << commands.front() << endl;
    // commands.popSafe();
    // cout << commands.front() << endl;
    // commands.popSafe();
    // cout << commands.front() << endl;
    // commands.popSafe();
    // cout << commands.front() << endl;
    // commands.popSafe();
    // cout << commands.empty() << endl;
    // Основной цикл
    while (!commands.empty())
    {
        if (commands.front() == '$')
        {                                              // Видим, что начался пакет
            commands.popSafe();                        // Избавляемся от "$"
            int command = isCommand(commands.front()); // Проверяем, существует ли команда из передаваемого пакета, а так же запоминаем саму команду
            if (command == -1)
            { // Вывод ошибки, если команда не существует
                cout << "Error: Некорректная команда" << endl;
                continue;
            }
            while (commands.front() != '#' && !commands.empty() && commands.front() != '$')
            {
                currentChecksum += commands.front(); // Считаем контрольную сумму передаваемого пакета
                commands.popSafe();
            }
            if (!commands.empty() && commands.front() == '$')
            { // Проверка на разрыв пакета
                currentChecksum = 0;
                cout << "Error: Обрыв передачи пакета" << endl;
                continue;
            }
            commands.popSafe();                            // Избавляемся от "#"
            checksum += charToInt(commands.front(), true); // Преобразование контрольной суммы пакета в удобный формат
            if (checksum == 17)
            { // Вывод ошибки, если контрольная сумма была указана неверно
                cout << "Error: Некорректная контрольная сумма" << endl;
                continue;
            }
            commands.popSafe();
            checksum += charToInt(commands.front(), false);
            if (charToInt(commands.front(), false) == 17)
            { // Вывод ошибки, если контрольная сумма была указана неверно
                cout << "Error: Некорректная контрольная сумма" << endl;
                continue;
            }
            commands.popSafe();
            if (currentChecksum == checksum)
            { // Проверка целостности пакета
                cout << "Пакет считан исправно!" << endl;
            }
            else
            {
                cout << "Error: Пакет передан некорректно" << endl;
            }
        }
        else
        {
            commands.popSafe();
        }
    }
}

int isCommand(char ch)
{
    if (ch == 't')
    {
        return 1;
    }
    else if (ch == 'c')
    {
        return 2;
    }
    else if (ch == 'r')
    {
        return 3;
    }
    else if (ch == 'i')
    {
        return 4;
    }
    else if (ch == 'd')
    {
        return 5;
    }
    else if (ch == 's')
    {
        return 6;
    }

    return -1;
}

int charToInt(char ch, bool x16)
{
    int num = 17;
    ch = tolower(ch);
    switch (ch)
    {
    case 'f':
        num = 0xf;
        break;
    case 'e':
        num = 0xe;
        break;
    case 'd':
        num = 0xd;
        break;
    case 'c':
        num = 0xc;
        break;
    case 'b':
        num = 0xb;
        break;
    case 'a':
        num = 0xa;
        break;
    case '9':
        num = 9;
        break;
    case '8':
        num = 8;
        break;
    case '7':
        num = 7;
        break;
    case '6':
        num = 6;
        break;
    case '5':
        num = 5;
        break;
    case '4':
        num = 4;
        break;
    case '3':
        num = 3;
        break;
    case '2':
        num = 2;
        break;
    case '1':
        num = 1;
        break;
    case '0':
        num = 0;
        break;
    }

    if (num != 17)
    {
        num = x16 ? num * 16 : num;
    }

    return num;
}