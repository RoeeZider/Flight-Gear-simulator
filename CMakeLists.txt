cmake_minimum_required(VERSION 3.15)
project(updatedSim)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(updatedSim main.cpp Command.cpp Command.h Expression.h Interpreter.cpp Interpreter.h)