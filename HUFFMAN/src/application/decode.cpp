//
// Created by kekosik on 13.05.18.
//

#include <RAII_ifstream.h>
#include <RAII_ofstream.h>
#include <generator.h>
#include <iostream>
#include "decode.h"


void decode(std::string const &a, std::string const &b) {
    RAII_ifstream reader(a);
    RAII_ofstream writer(b);
    uint16_t n = reader.getShort();
    if(n == 0){
        return;
    }
    if (n <= 2) {
        throw std::runtime_error("File compressed incorrectly, tree has less than 3 nodes");
    }
    std::vector<uchar> encodedTree;
    try {
        for (uint16_t i = 0; i < n; i += 8)
            encodedTree.emplace_back(reader.getChar());
    }
    catch(std::runtime_error const & e){
        std:: cerr << "File compressed incorrectly, no tree structure" << std::endl;
        return;
    }
    uint16_t leavesCount = (n + 1) / 2;
    std::vector<uchar> leaves;
    try {
        for (auto i = 0; i < leavesCount; i++)
            leaves.emplace_back(reader.getChar());
    }
    catch(std::runtime_error const & e){
        std::cerr << "File compressed incorrectly, no leave order" << std::endl;
        return;
    }
    uint64_t textSize = reader.getLong();
    generator huffmanTable;
    huffmanTable.generate(n, encodedTree, leaves, textSize);
    uchar now;
    char res;
    uint64_t cnt = 0;
    while (!reader.eof()) {
        now = reader.getChar();
        uchar lol = 0;
        for (int i = 0; i < 8; i++) {
            lol *= 2;
            lol += now % 2;
            now /= 2;
        }
        now = lol;
        for (int i = 0; i < 8; i++) {
            if(cnt == textSize)
                break;
            if (huffmanTable.getReverseCode(now % 2, res)) {
                writer.putChar(res);
                cnt++;
            }
            now /= 2;
        }
    }
}