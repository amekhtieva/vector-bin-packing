cmake_minimum_required(VERSION 3.17)

project(vector_bin_packing)

set(CMAKE_CXX_STANDARD 20)

add_library(lib STATIC
    src/algorithms/best_fit.cpp
    src/algorithms/bin_centric.cpp
    src/algorithms/first_fit.cpp
    src/algorithms/my.cpp
    src/algorithms/next_fit.cpp
    src/algorithms/solver.cpp
    src/algorithms/worst_fit.cpp

    src/library/bin.cpp
    src/library/item.cpp
    src/library/size.cpp
    src/library/utils.cpp

    src/tests/utils/utils.cpp
)

add_executable(large_test src/tests/large/main.cpp)
add_executable(my_test src/tests/my/main.cpp)
add_executable(small_test src/tests/small/main.cpp)
add_executable(triplets_test src/tests/triplets/main.cpp)

target_link_libraries(large_test lib)
target_link_libraries(my_test lib)
target_link_libraries(small_test lib)
target_link_libraries(triplets_test lib)
