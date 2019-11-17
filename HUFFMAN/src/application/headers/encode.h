//
// Created by kekosik on 13.05.18.
//

#ifndef HUFFMAN_ENCODE_H
#define HUFFMAN_ENCODE_H

#include <vector>
#include <string>
#include <constants.h>
#include <RAII_ifstream.h>
#include <RAII_ofstream.h>
#include <generator.h>
#include <OccurrencyMap.h>

OccurrencyMap accumulateCodes(std::string const &source);

void encode(std::string const &a, std::string const &b);

#endif //HUFFMAN_ENCODE_H
