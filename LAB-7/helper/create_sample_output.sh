#! bin/bash

# create a table to show the output of the script for
# head pos 500 1000 1500 2000 2500 3000 3500 4000 4500
# and algorithm 1, 2, 3, 4, 5, 6
# usage ./main -h <head pos> -a <algorithm>

make all

# clear output.txt
echo "" > sample_outputs.txt

for i in 1 2 3 4 5 6
do
    for j in 500 1000 1500 2000 2500 3000 3500 4000 4500
    do
        ./main -h $j -a $i >> sample_outputs.txt
        echo "" >> sample_outputs.txt
    done
done

make clean