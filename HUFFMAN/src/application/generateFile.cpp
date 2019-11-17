//
// Created by kekosik on 09.06.18.
//

#include <RAII_ofstream.h>
#include <string>
#include <random>
#include <chrono>
#include <ctime>

int main(int argc, char *argv[]) {
    RAII_ofstream writer = RAII_ofstream(std::string(argv[1]));
    std::mt19937 randomer(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    size_t text = atoi(argv[2]);
    for(size_t ind = 0; ind < text; ind++)
        writer.putChar(randomer());
}