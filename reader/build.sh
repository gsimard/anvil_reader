#!/bin/sh
g++ -lz endian.cpp tag.cpp anvil.cpp main.cpp -o reader && ./reader ../region/r.-1.0.mca