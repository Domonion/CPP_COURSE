//
// Created by kekosik on 13.05.18.
//

#ifndef HUFFMAN_RAII_IFSTREAM
#define HUFFMAN_RAII_IFSTREAM

#include <fstream>
#include <string>
#include <constants.h>

class RAII_ifstream {
public:
    RAII_ifstream() = delete;

    explicit RAII_ifstream(std::string const &filename);

    ~RAII_ifstream();

    bool eof();

    char getChar();

    uint16_t getShort();

    uint64_t getLong();

private:
    std::ifstream reader;
    std::string filename;
    char buffer[BUFFER_SIZE];
    size_t now, end;
};


#endif //HUFFMAN_RAII_IFSTREAM
