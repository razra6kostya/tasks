#!/bin/sh

gcc -g -Wall task243.c -o task243
if [ -f "task243" ]; then
    ./task243 < text.txt
fi
