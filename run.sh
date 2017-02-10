#!/bin/bash

rm -f conv
gcc -std=c99 -Werror -O3 -o conv conv.c
./conv 2 1280 720 1920 1080 4096 2160 