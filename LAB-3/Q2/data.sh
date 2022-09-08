#!/bin/bash

# j=0
function rand() {
    local i=1
    while [ $i -le 5 ]
    do
        R=$(($RANDOM%51))
        # R=$((i+j*5))
        echo $R
        i=$(($i+1))
    done
}

rm *.txt

i=1
while [ $i -le 5 ]
do
    rand > data_"$i".txt
    # j=$(($j+1))
    i=$(($i+1))
done