cmake_minimum_required(VERSION 3.17)

project(vector_bin_packing)

set(CMAKE_CXX_STANDARD 20)

add_executable(vector_bin_packing
    src/bin/main.cpp
    src/algorithms/solver.cpp src/algorithms/solver.h
    src/algorithms/first_fit.cpp src/algorithms/first_fit.h
    src/library/bin.cpp src/library/bin.h
    src/library/item.cpp src/library/item.h
    src/library/size.cpp src/library/size.h
)
