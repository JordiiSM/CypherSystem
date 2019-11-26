# cmake_minimum_required(VERSION <specify CMake version here>)
project(CypherSystem C)

set(CMAKE_C_STANDARD 99)

add_executable(CypherSystem main.c system.c system.h config.c config.h server.c server.h)