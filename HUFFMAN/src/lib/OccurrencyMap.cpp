//
// Created by kekosik on 13.05.18.
//

#include <OccurrencyMap.h>


void OccurrencyMap::add(uchar value) {
    count[value]++;
}

uint64_t OccurrencyMap::get(uchar value) const {
    return count[value];
}

void OccurrencyMap::set(uchar value, uint64_t amount) {
    count[value] = amount;
}

std::vector<uint64_t> const& OccurrencyMap::getVector() const {
    return count;
}

OccurrencyMap::OccurrencyMap() : count(256) {}

