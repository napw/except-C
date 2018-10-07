cmake_minimum_required(VERSION 3.12)
project(setjmp C)
set(CMAKE_C_STANDARD 99)

add_executable(setjmp main.c excepC.h exceptC.c)