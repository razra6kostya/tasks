#!/bin/sh

gcc -g -Wall gen_bin_file.c -o gen_bin_file
if [ gen_bin_file ]; then
    STR_BEGIN="bin_file"
    STR_END=".bin"
    I=0
    while [ $I -le 10 ]; do
        ./gen_bin_file $STR_BEGIN$I$STR_END
        sleep 1 
        I=$(( I + 1 ))
    done

    FILES_FOR_ANALYZE=$(ls -1 -v bin_file*)
    if [ -z "$FILES_FOR_ANALYZE" ]; then
        echo "No Files! Exit."
        exit 1
    fi
    
    gcc -Wall -g task254.c -o task254  
    if [ task254 ]; then
        ./task254 $FILES_FOR_ANALYZE analyze.txt
        cat analyze.txt
        rm bin_file* analyze.txt
    fi
fi
