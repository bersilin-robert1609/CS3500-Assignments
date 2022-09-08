#!/bin/bash

read pwd

LOWER=0
UPPER=0
SPECIAL=0
LENGTH=${#pwd}
DIGITS=0

i=0

while [ $i -lt $LENGTH ]
do
	case ${pwd:$i:1} in
		[a-z]) 								((LOWER++))			;;
		[A-Z])								((UPPER++))			;;
		[\!\@\#\$\%\^\&\*\(\)\-\+])			((SPECIAL++))		;;
		[0-9])								((DIGITS++))		;;
		*)									;;	
	esac
	
	((i++))
done

if [[ $LOWER -gt 0 && $UPPER -gt 0 && $SPECIAL -gt 0 ]]; then
	
	if [[ $LENGTH -ge 8 && $DIGITS -gt 0 ]]; then
		
		echo "The password is Strong"
	
	elif [[ $LENGTH -ge 6 ]]; then
		
		echo "The password is Moderate"
	
	fi
	
else 
	
	echo "The password is Weak"

fi
