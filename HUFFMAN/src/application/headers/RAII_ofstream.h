//
// Created by kekosik on 13.05.18.
//

#ifndef HUFFMAN_RAII_OFSTREAM
#define HUFFMAN_RAII_OFSTREAM

#include <string>
#include <fstream>
#include <ostream>
#include "constants.h"

class RAII_ofstream {
public:
    RAII_ofstream() = delete;

    explicit RAII_ofstream(std::string const &filename);

    ~RAII_ofstream();

    void putChar(char value);

    void putShort(uint16_t value);

    void putLong(uint64_t value);
private:
    std::ofstream writer;
    char buffer[BUFFER_SIZE];
    size_t now ;
};


#endif //HUFFMAN_RAII_OFSTREAM
