#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include "fifo.h"
#include <memory>

using namespace std;

// функция для вычисления контрольной суммы
int calculateChecksum(const string &data)
{
    uint8_t checksum = 0;
    for (auto i : data)
    {
        checksum += i;
    }

    return checksum;
}

int main()
{
    constexpr std::uint8_t len = 128;
    fifo::internal::FifoRaw<std::unique_ptr<std::uint16_t>, std::uint8_t, len>
        buf;

    queue<string> commands; // буфер для команд
    queue<string> results;  // буфер для результатов

    // добавляем несколько пакетов в буфер команд
    commands.push("$Команда 1#e6");
    commands.push("$Команда 2#e7");
    commands.push("$Команда 3#e8");

    // обрабатываем пакеты, записывая результаты в буфер
    while (!commands.empty())
    {
        // static char m[1024];
        //  извлекаем пакет из буфера команд
        string packet = commands.front();
        commands.pop();

        // проверяем корректность пакета
        if (packet[0] == '$' && packet[packet.length() - 3] == '#')
        {
            // если пакет корректный, выделяем из него данные и контрольную сумму
            string data = packet.substr(1, packet.length() - 4);
            int checksum = stoi(packet.substr(packet.find('#') + 1), nullptr, 16);

            // проверяем контрольную сумму
            if (calculateChecksum(data) == checksum)
            {
                // если контрольная сумма совпадает, обрабатываем команду и записываем результат в буфер
                string result = "Результат для " + data.substr(0, data.find("#"));
                results.push(result);
            }
            else
            {
                // если контрольная сумма не совпадает, выводим ошибку
                cout << "Ошибка: неверная контрольная сумма для пакета \"" << packet << "\""
                     << " checksum: " << std::hex << calculateChecksum(data) << " data: " << data << endl;
            }
        }
        else
        {
            // если пакет некорректный, выводим ошибку
            cout << "Ошибка: некорректный пакет \"" << packet << "\"" << endl;
        }
    }

    // выводим результаты
    while (!results.empty())
    {
        cout << results.front() << endl;
        results.pop();
    }

    return 0;
}