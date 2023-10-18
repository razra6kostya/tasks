#!/bin/sh

if [ -f "task421" ]; then
    make clean
fi
make run
./task421
