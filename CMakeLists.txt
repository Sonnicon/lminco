cmake_minimum_required(VERSION 3.23)
project(lminco)

set(CMAKE_CXX_STANDARD 14)

add_executable(lminco main.cpp main.h engine.cpp engine.h assembler.cpp assembler.h)
