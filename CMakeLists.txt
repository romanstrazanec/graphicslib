cmake_minimum_required(VERSION 3.12)
project(minGL
        VERSION 1.2.0
        DESCRIPTION "Simple C++ graphics library for Windows."
        HOMEPAGE_URL https://github.com/romanstrazanec/minGL)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(samples)
