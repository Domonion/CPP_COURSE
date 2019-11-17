cmake_minimum_required(VERSION 3.0)

project(BIGINT)

include_directories(${BIGINT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -fsanitize=address,undefined -Wall -Wshadow -Wextra -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3  -flto -fvisibility=hidden")

add_executable(big_integer_testing
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc
        vector.h
        test.cpp)

target_link_libraries(big_integer_testing -lpthread)

