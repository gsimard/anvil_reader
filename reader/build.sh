#!/bin/sh
g++ -g -lz endian.cpp tag.cpp anvil.cpp main.cpp -o reader && ./reader ../region/world2/r.-1.0.mca
