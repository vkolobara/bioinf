cmake_minimum_required(VERSION 3.5)
project(bioinf)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/format/FASTA.cpp src/format/FASTA.h src/algorithm/KMerGraph.cpp src/algorithm/KMerGraph.h src/format/SAM.cpp src/format/SAM.h)

add_executable(bioinf ${SOURCE_FILES})
