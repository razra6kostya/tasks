#!/bin/sh

gcc -Wall -g task246.c -o task246
if [ task246 ]; then
    ./task246 text.txt
fi
