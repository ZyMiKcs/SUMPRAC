#include <cctype>
#include <iomanip>
#include <iostream>
#include <queue>

#include "fifo.h"

using namespace std;

int isCommand(char ch);  // Проверка на валидность команды пакета
int charToInt(char ch, bool x16);  // Перевод шестнадцатеричного числа, записанного в char, в int
void startCommand(int command);

template <size_t Size> class PacketReader{
public:
enum class State{
    idle,
    command,
    size,
    dataL,
    dataH,
    checksum1,
    checksum2
};
enum class Command:uint8_t{
    TMS = 't',
    stableClocks = 'c',
    tlrReset = 'r',
    scanIr = 'i',
    scanDr = 'd',
    sleep = 's'
};
template <size_t inSize, size_t outSize> void execute(fifo::Fifo<uint8_t, inSize> &input, fifo::Fifo<uint8_t, outSize> &output)
{
    while(!input.empty())
    {
        auto ch = input.pop();
        switch (ch)
        {
        case '$':
            checksum = 0;
            state = State::command;
            index = 0;
            break;
        case '#':
            switch (state)
            {
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
            if (state==State::size){
                state=State::dataL;
            }
            break;
        default:
            if (state != State::checksum1 && state != State::checksum2) 
                checksum += ch;
            switch (state)
            {
            case State::idle:
                break;
            case State::command:
                if(isCommandCorrect(ch)) {
                    command = static_cast<Command>(ch);
                    state = State::size;
                } else {
                    output.push('-');
                    state = State::idle;
                }
                break;
            case State::size:
                try {
                    len = len*16+charToInt(ch,false);
                } catch (bad_exception &e) {
                    state = State::idle;
                }
                break;
            case State::dataL:
                try {
                    data[index] = charToInt(ch, false);
                    state = State::dataH;
                } catch (bad_exception &e) {
                    state = State::idle;
                }
                break;
            case State::dataH:
                try {
                    data[index] += charToInt(ch, true);
                    state = State::dataL;
                    index++;
                } catch (bad_exception &e) {
                    state = State::idle;
                }
                break;
            case State::checksum1:
                try {
                    checksumCorrect = charToInt(ch, true);
                    state = State::checksum2;
                } catch (bad_exception &e) {
                    state = State::idle;
                }
                break;
            case State::checksum2:
                try {
                    checksumCorrect += charToInt(ch, false);
                    if (checksumCorrect == checksum) {
                        output.push('+');
                    } else {
                        output.push('-');
                    }
                    state = State::idle;
                } catch (bad_exception &e) {
                    state = State::idle;
                }        
                break;
            }
            break;
        }
    }
}
PacketReader():state(State::idle), index(0), len(0), command(Command::sleep), checksum(0), checksumCorrect(0){}
private:
    State state;
    size_t index;
    uint16_t len;
    uint8_t data[Size/8];
    Command command;
    uint8_t checksum;
    uint8_t checksumCorrect;
bool isCommandCorrect(uint8_t ch){
    switch (static_cast<Command>(ch))
    {
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
};

int main(int argc, const char *argv[]) {
    constexpr std::uint8_t len = 128;
    using fifo::Fifo;
    using DataType = uint8_t;

    Fifo<DataType, len> commands;
    Fifo<DataType, len> result;
    PacketReader<1024> reader;

    for (int i = 1; i < argc; ++i) {
        const char *p = argv[i];
        while (*p) commands.push(*p++);
        reader.execute(commands,result);
        while(!result.empty()){
            cout<<static_cast<char>(result.pop());
        }
    }
    cout<<endl;
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