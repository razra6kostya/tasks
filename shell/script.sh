#!/bin/sh


gdb -ex "b main" -ex "run < text.txt" --args ./shell
