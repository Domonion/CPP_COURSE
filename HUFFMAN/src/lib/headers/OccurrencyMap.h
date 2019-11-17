//
// Created by kekosik on 13.05.18.
//

#ifndef HUFFMAN_DATA_OCCURENCY_MAP
#define HUFFMAN_DATA_OCCURENCY_MAP

#include <vector>
#include <cstdint>
#include <constants.h>

class OccurrencyMap {
public:
    OccurrencyMap();

    void add(uchar value);

    uint64_t get(uchar value) const;

    void set(uchar value, uint64_t amount);

    std::vector<uint64_t> const& getVector() const;

private:
    std::vector<uint64_t> count;
};


#endif //HUFFMAN_DATA_OCCURENCY_MAP
