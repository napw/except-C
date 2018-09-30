cmake_minimum_required(VERSION 3.12)
project(setjmp C)
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "-g ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-g")
endif (CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_C_STANDARD 99)

add_executable(setjmp main.c excepC.h exceptC.c)