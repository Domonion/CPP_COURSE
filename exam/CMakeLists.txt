cmake_minimum_required(VERSION 3.0)
project(exam)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -fsanitize=address,undefined -Wall -Wshadow -Wextra -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3  -flto -fvisibility=hidden")

add_executable(benchmark fixed_capacity_vector.cpp fixed_vector.h)
add_executable(tests vector_tests.cpp fixed_vector.h base_vector.h doctest/doctest.h)
