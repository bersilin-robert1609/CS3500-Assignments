#! /bin/bash

function getArray() {
    for file in *.txt
    do
        cat $file
    done
}

getArray | sort -n | awk 'NR==13'