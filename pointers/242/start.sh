#!/bin/sh
gcc -g -Wall task242.c -o task242
if [ "task242" ]; then
    ./task242 < text.txt
fi
