cmake_minimum_required(VERSION 3.5.1)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

FIND_PACKAGE( Boost 1.62.0 REQUIRED COMPONENTS system thread)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -pthread -pedantic -g")

add_executable (comb main.cpp)
target_link_libraries(comb ${Boost_LIBRARIES})

