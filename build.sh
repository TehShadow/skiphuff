#!/bin/bash
echo "Building compressor..."
g++ -std=c++17 main.cpp compressor.cpp skiplist.cpp -o compressor

echo "Building decoder..."
g++ -std=c++17 decoder.cpp compressor.cpp skiplist.cpp -o decoder

echo "Done!"