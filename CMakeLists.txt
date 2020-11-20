cmake_minimum_required(VERSION 3.17)
project(metaproga)

set(CMAKE_CXX_STANDARD 20)

add_executable(metaproga main.cpp TypeList.h Hierarchy.h Factory.h)