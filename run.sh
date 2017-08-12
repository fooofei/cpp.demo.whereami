#!/bin/bash


# If fail then exit
# Go library
mkdir bin
cd library
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release  .. || exit 1
make  || exit 1
cd ..
rm -rf build


cd ..
cd executable
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release  .. || exit 1
make  || exit 1
cd ..
rm -rf build
cd ..
cd bin
./call || exit 1