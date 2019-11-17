cmake_minimum_required(VERSION 2.8)
project(HUFFMAN_LIBRARY)

include_directories(${HUFFMAN_LIBRARY_SOURCE_DIR}/headers)
include_directories(${HUFFMAN_LIBRARY_SOURCE_DIR}/gtest)

set(LIB_HEADERS headers/generator.h
        headers/constants.h
        headers/OccurrencyMap.h)

set(LIB_SOURCES generator.cpp
        OccurrencyMap.cpp)

add_library(huf_lib STATIC
        ${LIB_HEADERS} ${LIB_SOURCES})

add_executable(testlib
        gtest/gtest_main.cc
        gtest/gtest-all.cc
        gtest/gtest.h
        test.cpp)

target_link_libraries(testlib huf_lib -lpthread)