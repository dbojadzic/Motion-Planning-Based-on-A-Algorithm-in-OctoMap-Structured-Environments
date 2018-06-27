cmake_minimum_required(VERSION 3.10)
project(Rosettacode)

set(CMAKE_CXX_STANDARD 11)

add_executable(Rosettacode main.cpp point.h map.h node.h aStar.h )