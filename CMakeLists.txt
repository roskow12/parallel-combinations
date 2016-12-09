cmake_minimum_required(VERSION 3.5.1)

option(MULTITHREADED_SUPPORT_ENABLES "Multithreaded Support Enabled" TRUE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -pthread -pedantic -g")

file(GLOB_RECURSE Comb_SOURCES "src/*.cpp")

add_executable (comb ${Comb_SOURCES})


