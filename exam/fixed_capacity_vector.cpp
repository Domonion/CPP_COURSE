//
// Created by domonion on 12.09.18.
//

#include "fixed_vector.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>
#include <ctime>
#include <vector>

int main() {
    const int iterations = 1000;
    uint64_t fixed_capacity_vector_clocks = 0;
    uint64_t stl_vector_clocks = 0;
    uint64_t stl_vector_with_reserved_capacity_clocks = 0;
    for (int temp = 0; temp < iterations; temp++) {
        const int N = 1000000;
        time_t now = clock();
        fixed_vector<int, N> a;
        for (int i = 0; i < N; i++)
            a.push_back(i);
        for (int i = 0; i < N; i++)
            a.pop_back();
        time_t res = clock();
        fixed_capacity_vector_clocks += (res - now);
        now = clock();
        std::vector<int> b;
        for (int i = 0; i < N; i++)
            b.push_back(i);
        for (int i = 0; i < N; i++)
            b.pop_back();
        res = clock();
        stl_vector_clocks += res - now;
        now = clock();
        std::vector<int> c;
        c.reserve(N);
        for (int i = 0; i < N; i++)
            c.push_back(i);
        for (int i = 0; i < N; i++)
            c.pop_back();
        res = clock();
        stl_vector_with_reserved_capacity_clocks += res - now;
    }
    std::cout << "Performance benchmark\nfixed_capacity_vector: "
              << (long double) fixed_capacity_vector_clocks / CLOCKS_PER_SEC
              << " sec\nstl vector: " << (long double) stl_vector_clocks / CLOCKS_PER_SEC
              << " sec\nstl vector with reserved capacity "
              << (long double) stl_vector_with_reserved_capacity_clocks / CLOCKS_PER_SEC << " sec";
    return 0;
}
