//
// Created by kekosik on 13.05.18.
//

#include <iostream>
#include "encode.h"
#include "decode.h"


int main(int argc, char *argv[]) {
    if (argc != 4) {
        if (argc == 2 && std::string(argv[1]) == "-help")
            std::cerr << "Usage: c|d source-path destination-path";
        else
            std::cerr << "Incorrect arguments\n";
        return 0;
    }
    std::string mode = argv[1];
    if (mode != "c" && mode != "d") {
        std::cerr << "Incorrect mode\n";
        return 0;
    }
    std::string action = (mode == "c" ? "Compression" : "Decompression");
    std::cout << action << " started\n";
    time_t timer = clock();
    try {
        if (mode == "c")
            encode(argv[2], argv[3]);
        else
            decode(argv[2], argv[3]);
    }
    catch (const std::runtime_error e) {//как и что ловить
        std::cerr << action << " failed.\n" << e.what();
        return 0;
    }
    std::cout << (long double)(clock() - timer) / CLOCKS_PER_SEC << std::endl;
    std::cout << action << " succeeded";
    return 0;
}