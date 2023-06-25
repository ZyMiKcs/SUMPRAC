#include <cctype>
#include <iomanip>
#include <iostream>
#include <fstream>

#include "fifo.h"

// hello
using namespace std;

int charToInt(char ch, bool x16);  // Перевод шестнадцатеричного числа, записанного в char, в int


template <size_t Size>
class PacketReader {
   public:
    enum class State { idle, command, size, dataL, dataH, checksum1, checksum2 };
    enum class Command : uint8_t {
        TMS = 't',
        stableClocks = 'c',
        tlrReset = 'r',
        scanIr = 'i',
        scanDr = 'd',
        sleep = 's'
    };
    template <size_t inSize, size_t outSize>
    void execute(fifo::Fifo<uint8_t, inSize> &input, fifo::Fifo<uint8_t, outSize> &output) {
        while (!input.empty()) {
            auto ch = input.pop();
            switch (ch) {
                case '$':
                    checksum = 0;
                    state = State::command;
                    index = 0;
                    len = 0;
                    checksumCorrect = 0;
                    break;
                case '#':
                    switch (state) {
                        case State::idle:
                        case State::command:
                        case State::checksum1:
                        case State::checksum2:
                            state = State::idle;
                            break;
                        case State::size:
                        case State::dataL:
                        case State::dataH:
                            state = State::checksum1;
                            break;
                    }
                    break;

                case ',':
                    checksum += ch;
                    if (state == State::size) {
                        if (len != 0) {
                            state = State::dataL;
                        } else {
                            cout << "Попытка передачи некорректного пакета" << endl;
                            output.push('-');
                            state = State::idle;
                        }
                    }
                    break;
                default:
                    if (state != State::checksum1 && state != State::checksum2) checksum += ch;
                    switch (state) {
                        case State::idle:
                            break;
                        case State::command:
                            if (isCommandCorrect(ch)) {
                                command = static_cast<Command>(ch);
                                state = State::size;
                            } else {
                                cout << "Попытка передачи некорректной команды" << endl;
                                output.push('-');
                                state = State::idle;
                            }
                            break;
                        case State::size:
                            try {
                                len = len * 16 + charToInt(ch, false);
                            } catch (bad_exception &e) {
                                cout << "Попытка передачи некорректного размера" << endl;
                                output.push('-');
                                state = State::idle;
                            }
                            break;
                        case State::dataL:
                            try {
                                data[index] = charToInt(ch, false);
                                state = State::dataH;
                            } catch (bad_exception &e) {
                                cout << "Попытка передачи некорректных данных" << endl;
                                output.push('-');
                                state = State::idle;
                            }
                            break;
                        case State::dataH:
                            try {
                                isArrEmpty = false;
                                data[index] += charToInt(ch, true);
                                state = State::dataL;
                                index++;
                            } catch (bad_exception &e) {
                                cout << "Попытка передачи некорректных данных" << endl;
                                output.push('-');
                                state = State::idle;
                            }
                            break;
                        case State::checksum1:
                            try {
                                checksumCorrect = charToInt(ch, true);
                                state = State::checksum2;
                            } catch (bad_exception &e) {
                                cout << "Попытка передачи некорректной контрольной суммы" << endl;
                                output.push('-');
                                state = State::idle;
                            }
                            break;
                        case State::checksum2:
                            if (commandArgsCount(command) == 0 && len != 0) {
                                cout << "Попытка передачи некорректного пакета (команда не должна иметь аргументы)" << endl;
                                output.push('-');
                                state = State::idle;
                                break;
                            }
                            if (commandArgsCount(command) == 1 && !isArrEmpty) {
                                cout << "Попытка передачи некорректного пакета (команда должна иметь один аргумент)" << endl;
                                output.push('-');
                                state = State::idle;
                                break;
                            }
                            if (commandArgsCount(command) == 2 && (len == 0 || isArrEmpty)) {
                                cout << "Попытка передачи некорректного пакета (команда содержит не все аргументы)" << endl;
                                output.push('-');
                                state = State::idle;
                                break;
                            }
                            try {
                                checksumCorrect += charToInt(ch, false);
                                if (checksumCorrect == checksum) {
                                    startCommand(command, len, data);
                                    output.push('+');
                                } else {
                                    cout << "Попытка передачи некорректной контрольной суммы " << hex << static_cast<int>(checksum) << endl;
                                    output.push('-');
                                }
                                state = State::idle;
                            } catch (bad_exception &e) {
                                cout << "Попытка передачи некорректной контрольной суммы" << endl;
                                output.push('-');
                                state = State::idle;
                            }
                            break;
                    }
                    break;
            }
        }
    }

    PacketReader()
        : state(State::idle), index(0), len(0), command(Command::sleep), checksum(0), checksumCorrect(0), isArrEmpty(true) {}

   private:
    State state;
    size_t index;
    uint16_t len;
    uint8_t data[Size / 8];
    Command command;
    uint8_t checksum;
    uint8_t checksumCorrect;
    bool isArrEmpty;

    bool isCommandCorrect(uint8_t ch) {
        switch (static_cast<Command>(ch)) {
            case Command::TMS:
            case Command::stableClocks:
            case Command::tlrReset:
            case Command::scanIr:
            case Command::scanDr:
            case Command::sleep:
                return true;
        }
        return false;
    }

    int commandArgsCount(PacketReader<Size>::Command cmd) {
        switch(cmd) {
            case Command::TMS:
            case Command::scanDr:
            case Command::scanIr:
                return 2;
            case Command::stableClocks:
            case Command::sleep:
                return 1;
            case Command::tlrReset:
                return 0;
        }

        return 0;
    }
    
    void getCommandName(PacketReader<Size>::Command cmd, ofstream& outputFile) {
        switch(cmd) {
            case Command::TMS:
                outputFile << "TMS";
                break;
            case Command::scanDr:
                outputFile << "DR_SCAN";
                break;
            case Command::scanIr:
                outputFile << "IR_SCAN";
                break;
            case Command::stableClocks:
                outputFile << "STABLE_CLOCKS";
                break;
            case Command::sleep:
                outputFile << "SLEEP";
                break;
            case Command::tlrReset:
                outputFile << "TLR_RESET";
                break;
        }
    }

    void startCommand(PacketReader<Size>::Command cmd, uint16_t size, uint8_t *data) {
        ofstream outputFile;
        outputFile.open("output", ios::app);
        getCommandName(cmd, outputFile);
        outputFile << "," << size << ",";

        for (int i = 0; i < size; i++) {
            outputFile << std::hex << static_cast<int>((data[i/8]>>(i%8))&1);
        }
        outputFile << endl;
        outputFile.close();
    }
};


int main(int argc, const char *argv[]) {
    fstream outputFile;
    outputFile.open("output", ios::out);
    outputFile.close();
    constexpr std::uint8_t len = 128;
    using fifo::Fifo;
    using DataType = uint8_t;

    Fifo<DataType, len> commands;
    Fifo<DataType, len> result;
    PacketReader<1024> reader;

    for (int i = 1; i < argc; ++i) {
        const char *p = argv[i];
        while (*p) commands.push(*p++);
        reader.execute(commands, result);
        while (!result.empty()) {
            cout << static_cast<char>(result.pop()) << endl;
        }
    }
    cout << endl;
}

int charToInt(char ch, bool x16) {
    if (ch >= '0' && ch <= '9') {
        return (ch - '0') * (x16 ? 16 : 1);
    } else if (ch >= 'a' && ch <= 'f') {
        return (ch - 'a' + 10) * (x16 ? 16 : 1);
    } else if (ch >= 'A' && ch <= 'F') {
        return (ch - 'A' + 10) * (x16 ? 16 : 1);
    } else
        throw std::bad_exception();
}
