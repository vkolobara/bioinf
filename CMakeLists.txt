cmake_minimum_required(VERSION 3.8)
project(bioinf)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/format/PAF.cpp src/format/FASTQ.cpp src/format/FASTQ.h src/format/FASTA.cpp src/format/FASTA.h)

add_executable(bioinf ${SOURCE_FILES})