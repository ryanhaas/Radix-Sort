#!/bin/sh

# Script for automatically generating an executable for C++ code
# $1 = Name of cc file (without cc extension)

mkdir $1_dir
g++ -Wall -O3 -fopenmp $1.cc -o $1_dir/$1
./$1_dir/$1
