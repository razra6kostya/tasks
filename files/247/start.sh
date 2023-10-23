#!/bin/sh

gcc -Wall -g task247.c -o task247
if [ task247 ]; then
    ./task247 text.txt
fi
