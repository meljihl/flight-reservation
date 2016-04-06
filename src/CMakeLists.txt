cmake_minimum_required(VERSION 2.8.3)

#Here we add a Cmake Module abble to find GSOAP on your system, and find it
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
#find_package(Doxygen)
add_subdirectory(rest)
add_subdirectory(soap)

