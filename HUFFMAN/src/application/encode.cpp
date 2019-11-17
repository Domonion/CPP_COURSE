//
// Created by kekosik on 13.05.18.
//

#include "encode.h"
#include <iostream>

OccurrencyMap accumulateCodes(std::string const &source) {
    RAII_ifstream reader(source);
    OccurrencyMap res;
    while (!reader.eof()) {
        res.add(reader.getChar());
    }
    return res;
}

void encode(std::string const &source, std::string const &destination) {
    generator huffmanTable(accumulateCodes(source));
    RAII_ifstream reader(source);
    RAII_ofstream writer(destination);
    uint16_t n = huffmanTable.getTreeSize();
    if(n == 0){
        writer.putShort(n);
        return;
    }
    std::vector<uchar> treeEncode = huffmanTable.encodeTree();
    std::vector<uchar> treeLeaves = huffmanTable.encodeTreeLeaves();
    uint64_t text = huffmanTable.getTextSize();
    writer.putShort(n);
    for (char x : treeEncode)
        writer.putChar(x);
    for (char x : treeLeaves)
        writer.putChar(x);
    writer.putLong(text);
    uchar out = 0;
    uchar ind = 0;
    while (!reader.eof()) {
        std::vector<bool> const & code = huffmanTable.getCode(reader.getChar());
        for (size_t i = 0; i < code.size(); i++) {
            out <<= 1;
            out += code[i];
            ind++;
            if (ind == 8) {
                writer.putChar(out);
                ind = 0;
                out = 0;
            }
        }
    }
    if (ind != 0)
        writer.putChar(out << (8 - ind));
}