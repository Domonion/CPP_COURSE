cmake_minimum_required(VERSION 3.10)
project(HUFFMAN_APPLICATION)

include_directories(${HUFFMAN_LIBRARY_SOURCE_DIR}/headers)
include_directories(${HUFFMAN_APPLICATION_SOURCE_DIR}/headers)

set(HEADERS headers/RAII_ifstream.h
        headers/RAII_ofstream.h
        headers/encode.h
        headers/decode.h)

set(SOURCES RAII_ifstream.cpp
        RAII_ofstream.cpp
        encode.cpp
        decode.cpp)

add_executable(HUFFMAN ${HEADERS} ${SOURCES} main.cpp)

target_link_libraries(HUFFMAN huf_lib)

add_executable(generateFile generateFile.cpp headers/RAII_ofstream.h RAII_ofstream.cpp)