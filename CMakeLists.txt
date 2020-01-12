cmake_minimum_required(VERSION 3.13)
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()
set (CMAKE_CXX_STANDARD 11)
project(neuralnet)
#set(EXECUTABLE_OUTPUT_PATH ~/etc/etc/usr/etc)

#The includes
include_directories(include)

#file(GLOB SOURCES src/*.cpp)

#remove from glob
#list(REMOVE_ITEM SOURCES badcode.cpp)

add_executable(net neuralnet.c)