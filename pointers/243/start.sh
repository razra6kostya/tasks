#!/bin/sh

if [ -f "task243" ]; then
    make clean
fi
make run
./task243 < text.txt
