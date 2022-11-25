# cmake_minimum_required(VERSION <specify CMake version here>)
project(webserver C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(lock)

add_executable(webserver
        lock/locker.h)
