//
// Created by kekosik on 13.05.18.
//

#include "RAII_ifstream.h"

RAII_ifstream::RAII_ifstream(std::string const &filename) : reader(filename, std::ifstream::binary), filename(filename), now(0),
                                                       end(0) {
    if (reader.fail()) {
        reader.close();
        throw std::runtime_error("Cannot read file\nFilename: " + filename);
    }
}

RAII_ifstream::~RAII_ifstream() {
    reader.close();
}

bool RAII_ifstream::eof() {
    if(now == end){
        reader.read(buffer, BUFFER_SIZE);
        end = static_cast<size_t>(reader.gcount());
        now = 0;
    }
    return end == 0;
}

char RAII_ifstream::getChar() {
    if (eof())
        throw std::runtime_error("Data expected, but found EOF\nFilename: " + filename);
    return buffer[now++];
}

uint16_t RAII_ifstream::getShort() {
    uint16_t res = 0;
    for (int i = 0; i < 2; i++)
        res <<= 8, res += uchar(getChar());
    return res;
}

uint64_t RAII_ifstream::getLong() {
    uint64_t res = 0;
    for (int i = 0; i < 4; i++)
        res <<= 16, res += getShort();
    return res;
}