#!/bin/sh

gcc -g -Wall task505.c -o task505
if [ task505 ]; then
    python3 test_task505.py
fi
