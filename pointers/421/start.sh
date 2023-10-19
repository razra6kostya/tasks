#!/bin/sh

if [ -f "task421" ]; then
    make clean
fi
make run
valgrind ./task421
./task421
