#!/bin/bash

for (( i=1 ; i <= 9; i++ ))
do
	for (( j=1 ; j <= 9; j++ ))
	do
		if [ $i -le $j ]
		then
			result=$[ $i * $j ]
			echo -n "$i * $j = $result   "
		fi
	done
	echo
done
