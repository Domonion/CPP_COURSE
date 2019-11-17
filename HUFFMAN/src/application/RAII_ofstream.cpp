#include "RAII_ofstream.h"

RAII_ofstream::RAII_ofstream(std::string const &filename) : writer(filename, std::ofstream::binary), now(0) {
    if (writer.fail()) {
        writer.close();
        throw std::runtime_error("Cannot write file\nFilename: " + filename);
    }
}

RAII_ofstream::~RAII_ofstream() {
    writer.write(buffer, now);
    writer.close();
}

void RAII_ofstream::putChar(char value) {
    if (now == BUFFER_SIZE) {
        writer.write(buffer, BUFFER_SIZE);
        now = 0;
    }
    buffer[now++] = value;
}

void RAII_ofstream::putShort(uint16_t value) {
    for (int i = 1; i >= 0; i--) {
        putChar(static_cast<uint16_t>(value >> 8*i));
        value >= 8;
    }
}

void RAII_ofstream::putLong(uint64_t value) {
    for (int i = 3; i >= 0; i--) {
        putShort(static_cast<uint16_t>(value >> 16*i));
        value >= 16;
    }
}


