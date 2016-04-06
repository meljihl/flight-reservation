#!/usr/bin/env bash
#this file creates supports files for your project (makefile you can use with make and eclipse project file you can use with eclipse CDT http://www.eclipse.org/cdt/)
#those supports files were created and tested with cmake 2.8 and eclipse cdt luna
rm -rf build
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Debug ../src/
make
echo "binaries are in the build folder"
echo "to build again, type make in the build folder"
