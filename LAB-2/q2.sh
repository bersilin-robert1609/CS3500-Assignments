#!/bin/bash

declare -a array tempArr

merge_sort()
{
    if [[ $1 < $2 ]]; then #entry check
        local mid=$((($1+$2)/2))
        
        #recursive calls
        merge_sort $1 $mid
        merge_sort $(($mid+1)) $2
        
        #sorting
        local i=$1;
        local j=$(($mid+ 1))
        local k=$((0))

        while ((  $i <= $mid  &  $j <= $2  ))
        do
            if [[ $((array[$i])) < $((array[$j])) ]]; then
                tempArr[$k]=$((array[$i]));
                k=$(($k+1));
                i=$(($i+1));
            else
                tempArr[$k]=$((array[$j]));
                k=$(($k+1));
                j=$(($j+1));
            fi
        done

        while (( $i <= $mid ))
        do
            tempArr[$k]=$((array[$i]));
            k=$(($k+1));
            i=$(($i+1));
        done
        while (( $j <= $2 ))
        do
            tempArr[$k]=$((array[$j]));
            k=$(($k+1));
            j=$(($j+1));
        done

        i=$1;
        k=$((0)); 
        while (( $i <= $2 ))
        do
            array[$i]=$((tempArr[$k]));
            i=$(($i+1))
            k=$(($k+1))
        done
    fi
}

#receving inputs and calling mergesort function
read n

for(( c = 0 ; c < $n ; c++ ))
do
    read array[$c]
    tempArr[$c]=$array[$c]
done

merge_sort 0 $(($n-1))

echo -e "${array[@]}"
