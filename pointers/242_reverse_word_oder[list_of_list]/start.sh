#!/bin/sh

if [ -f "task242" ]; then
    make clean
fi
make run
./task242 < text.txt
