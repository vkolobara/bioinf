cmake_minimum_required(VERSION 3.5)
project(bioinf)

set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCE_FILES src/main.cpp src/algorithm/KMerGraph.cpp src/format/FASTA.cpp src/format/SAM.cpp)

add_executable(bioinf ${SOURCE_FILES})
