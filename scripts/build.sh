#!/bin/bash

# Cleanup previous build dir
rm -rf build/
mkdir build/
cd build/

# Build
cmake ..
make