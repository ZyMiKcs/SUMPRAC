#include <cctype>
#include <iomanip>
#include <iostream>
#include <queue>

#include "fifo.h"

using namespace std;

int isCommand(char ch);  // Проверка на валидность команды пакета
int charToInt(char ch, bool x16);  // Перевод шестнадцатеричного числа, записанного в char, в int
void startCommand(int command);

int main(int argc, const char *argv[]) {
    constexpr std::uint8_t len = 128;
    using DataType = char;
    using SizeType = uint8_t;

    fifo::internal::FifoRaw<DataType, SizeType, len> commands;
    fifo::internal::FifoRaw<DataType, SizeType, len> result;

    int currentChecksum = 0;  // Значение контрольной суммы текущего пакета
    int checksum = 0;  // Контрольное значение пакета, которое было указано заранее

    for (int i = 1; i < argc; ++i) {
        const char *p = argv[i];
        while (*p) commands.push(*p++);

        while (!commands.empty()) {
            // auto ch = commands.pop();
            if (commands.front() == '$') {  // Видим, что начался пакет
                commands.popSafe();         // Избавляемся от "$"
                int command =
                    isCommand(commands.front());  // Проверяем, существует ли команда из передаваемого пакета,
                                                  // а так же запоминаем саму команду
                if (command == -1) {  // Вывод ошибки, если команда не существует
                    cout << "Error: Некорректная команда" << endl;
                    continue;
                }
                while (commands.front() != '#' && !commands.empty() && commands.front() != '$') {
                    currentChecksum += commands.front();  // Считаем контрольную сумму передаваемого пакета
                    commands.popSafe();
                }
                if (!commands.empty() && commands.front() == '$') {  // Проверка на разрыв пакета
                    currentChecksum = 0;
                    cout << "Error: Обрыв передачи пакета" << endl;
                    continue;
                }
                commands.popSafe();  // Избавляемся от "#"
                try {
                    checksum = charToInt(commands.front(),
                                         true);  // Преобразование контрольной суммы пакета в удобный формат
                } catch (std::bad_exception &e) {
                    cout << "Error: Некорректная контрольная сумма" << endl;
                }
                commands.popSafe();  // Избавляемся от "#"
                try {
                    checksum += charToInt(commands.front(),
                                          false);  // Преобразование контрольной суммы пакета в удобный формат
                } catch (std::bad_exception &e) {
                    cout << "Error: Некорректная контрольная сумма" << endl;
                }
                commands.popSafe();
                if (currentChecksum == checksum) {  // Проверка целостности пакета
                    cout << "Пакет считан исправно!" << endl;
                    startCommand(command);
                } else {
                    cout << "Error: Пакет передан некорректно" << endl;
                }
            } else {
                commands.popSafe();
            }
        }
    }
}

int isCommand(char ch) {
    if (ch == 't') {
        return 1;
    } else if (ch == 'c') {
        return 2;
    } else if (ch == 'r') {
        return 3;
    } else if (ch == 'i') {
        return 4;
    } else if (ch == 'd') {
        return 5;
    } else if (ch == 's') {
        return 6;
    }

    return -1;
}

void startCommand(int command) {
    cout << "Выполняю команду: ";
    switch (command) {
        case 1:
            cout << "Смена TMS" << endl;
            break;
        case 2:
            cout << "STABLE_CLOCKS" << endl;
            break;
        case 3:
            cout << "TLR_RESET" << endl;
            break;
        case 4:
            cout << "SCAN_IR" << endl;
            break;
        case 5:
            cout << "SCAN_DR" << endl;
            break;
        case 6:
            cout << "SLEEP" << endl;
            break;
    }
}

int charToInt(char ch, bool x16) {
    ch = tolower(ch);
    if (ch >= '0' && ch <= '9') {
        return (ch - '0') * (x16 ? 16 : 1);
    } else if (ch >= 'a' && ch <= 'f') {
        return (ch - 'f') * (x16 ? 16 : 1);
    } else if (ch >= 'A' && ch <= 'F') {
        return (ch - 'A') * (x16 ? 16 : 1);
    } else
        throw std::bad_exception();
}